package com.avalon.payment;

import android.util.Log;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;

import com.samsungapps.plasma.*;

import org.cocos2dx.lib.Cocos2dxActivity;
import com.avalon.payment.Backend;

public class PurchasingObserver implements PlasmaListener
{
    private static final int ITEM_AMOUNT = 25;
    private static final String TAG = "avalon.payment.PurchasingObserver";
    private Cocos2dxActivity activity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
    private Plasma plasma = null;
    private int transactionId = 0;
    private int taskCount = 0;
    private Map<String, Boolean> productIds;
    public static String itemGroupId;

    public PurchasingObserver()
    {
        if (itemGroupId.isEmpty()) {
            activity.runOnGLThread(new Runnable() {
                public void run() {
                    throw new RuntimeException("Required itemGroupId not set!");
                }
            });
        }
        
        plasma = new Plasma(itemGroupId, activity);
        plasma.setPlasmaListener(this);
        plasma.setShowProgressDialog(false);

        threadOnInitialized();
    }

    public void startItemDataRequest(Map<String, Boolean> productIds)
    {
        this.productIds = productIds;
        final PurchasingObserver self = this;
        activity.runOnUiThread(new Runnable() {
            public void run() {
                self.plasma.requestItemInformationList(self.transactionId++, 1, ITEM_AMOUNT);
            }
        });
    }

    public void purchase(final String productId, boolean isConsumable)
    {
        final PurchasingObserver self = this;
        activity.runOnUiThread(new Runnable() {
            public void run() {
                if (!self.plasma.requestPurchaseItem(self.transactionId++, productId)) {
                    Log.v(TAG, "purchase failed: SKU - " + productId);
                } else {
                    Log.v(TAG, "purchase started: SKU - " + productId);
                    self.threadIncrementTaskCounter();
                }
            }
        });
    }

    /**
     *
     * Plasma callbacks
     *
     */

    @Override
    public void onItemInformationListReceived(int transactionId, int statusCode, ArrayList<ItemInformation> itemInformationList)
    {
        if (statusCode != Plasma.STATUS_CODE_SUCCESS) {
            Log.v(TAG, "onItemInformationListReceived failed -- PAYMENT SERVICE NOT STARTED!");
            return;
        }

        for (ItemInformation itemInformation : itemInformationList) {
            threadDelegateOnItemData(itemInformation);
        }

        final PurchasingObserver self = this;
        activity.runOnUiThread(new Runnable() {
            public void run() {
                self.plasma.requestPurchasedItemInformationList(self.transactionId++, 1, ITEM_AMOUNT);
            }
        });
    }

    @Override
    public void onPurchasedItemInformationListReceived(int transactionId, int statusCode, ArrayList<PurchasedItemInformation> purchasedItemInformationList)
    {
        if (statusCode != Plasma.STATUS_CODE_SUCCESS) {
            Log.v(TAG, "onPurchasedItemInformationListReceived failed -- PAYMENT SERVICE NOT STARTED!");
            return;
        }

        for (PurchasedItemInformation purchaseItemInformation : purchasedItemInformationList) {
            threadDelegateOnItemData(purchaseItemInformation);
        }

        threadDelegateOnServiceStarted();

        // yes -- a second loop -- but OnPurchaseSucceed() must be called after
        // all products are filled with their metadata!
        for (PurchasedItemInformation purchaseItemInformation : purchasedItemInformationList) {
            if (!isConsumable(purchaseItemInformation)) {
                threadDelegateOnPurchaseSucceed(purchaseItemInformation);
            }
        }
    }

    @Override
    public void onPurchaseItemInitialized(int transactionId, int statusCode, PurchaseTicket purchaseTicket)
    {
        if (statusCode != Plasma.STATUS_CODE_SUCCESS) {
            Log.v(TAG, "onPurchaseItemInitialized failed");
            threadDelegateOnPurchaseFail();
            threadDecrementTaskCounter();
        }
    }

    @Override
    public void onPurchaseItemFinished(int transactionId, int statusCode, PurchasedItemInformation purchaseItemInformation)
    {
        if (statusCode != Plasma.STATUS_CODE_SUCCESS) {
            Log.v(TAG, "onPurchaseItemFinished failed");
            threadDelegateOnPurchaseFail();
            threadDecrementTaskCounter();
        } else {
            threadDelegateOnItemData(purchaseItemInformation);
            threadDelegateOnPurchaseSucceed(purchaseItemInformation);
            threadDecrementTaskCounter();
        }
    }

    /**
     *
     * Helper methods that keeps to code above short and readable
     *
     */

    private void threadOnInitialized()
    {
        activity.runOnGLThread(new Runnable() {
            public void run() {
                Backend.onInitialized();
            }
        });
    }

    private void threadDelegateOnServiceStarted()
    {
        activity.runOnGLThread(new Runnable() {
            public void run() {
                Backend.delegateOnServiceStarted();
            }
        });
    }
    
    private void threadDelegateOnItemData(final ItemInformation itemInformation)
    {
        activity.runOnGLThread(new Runnable() {
            public void run() {
                Backend.delegateOnItemData(
                    itemInformation.getItemId(),
                    itemInformation.getItemName(),
                    itemInformation.getItemDescription(),
                    itemInformation.getItemPriceString(),
                    (float)itemInformation.getItemPrice()
                );
            }
        });
    }

    private void threadDecrementTaskCounter()
    {
        if (--taskCount == 0) {
            activity.runOnGLThread(new Runnable() {
                public void run() {
                    Backend.delegateOnTransactionEnd();
                }
            });
        }
    }

    private void threadIncrementTaskCounter()
    {
        if (++taskCount == 1) {
            activity.runOnGLThread(new Runnable() {
                public void run() {
                    Backend.delegateOnTransactionStart();
                }
            });
        }
    }
    
    private void threadDelegateOnPurchaseSucceed(final PurchasedItemInformation purchaseItemInformation)
    {
        activity.runOnGLThread(new Runnable() {
            public void run() {
                Backend.delegateOnPurchaseSucceed(purchaseItemInformation.getItemId());
            }
        });
    }

    private void threadDelegateOnPurchaseFail()
    {
        activity.runOnGLThread(new Runnable() {
            public void run() {
                Backend.delegateOnPurchaseFail();
            }
        });
    }

    private boolean isConsumable(ItemInformation itemInformation)
    {
        String productId = itemInformation.getItemId();
        return (productIds.containsKey(productId) && productIds.get(productId));
    }
}
