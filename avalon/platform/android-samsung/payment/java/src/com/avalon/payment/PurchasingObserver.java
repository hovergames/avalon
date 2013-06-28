package com.avalon.payment;

<<<<<<< HEAD
import android.util.Log;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;

import com.samsungapps.plasma.*;

import org.cocos2dx.lib.Cocos2dxActivity;
import com.avalon.payment.Backend;

=======
import com.samsungapps.plasma.*;

>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
public class PurchasingObserver implements PlasmaListener
{
    private static final int ITEM_AMOUNT = 25;
    private static final String TAG = "avalon.payment.PurchasingObserver";
<<<<<<< HEAD
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
=======
    private Plasma plasma = null;
    private int transactionId = 0;
    private Set<Integer> pendingTransactions = new HashSet<Integer>();
    private Set<String> ownedItems = new HashSet<String>();

    /**
     * ---- PLASMA INTERFACE START ----
>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
     */

    @Override
    public void onItemInformationListReceived(int transactionId, int statusCode, ArrayList<ItemInformation> itemInformationList)
    {
<<<<<<< HEAD
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
=======
        // Not used as we don't expose the abillity to list / show all available
        // items in out itemGroup.
>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
    }

    @Override
    public void onPurchasedItemInformationListReceived(int transactionId, int statusCode, ArrayList<PurchasedItemInformation> purchasedItemInformationList)
    {
<<<<<<< HEAD
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
=======
        // Contains a list of all purchased items in our itemGroup. Triggered
        // via requestPurchasedItemInformationList() after each complete
        // purchase to keep the local item DB in sync.

        itemDbClear();
        for (PurchasedItemInformation itemInformation : purchasedItemInformationList) {
            itemDbAdd(itemInformation.getItemId());
        }
        itemDbSave();
>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
    }

    @Override
    public void onPurchaseItemInitialized(int transactionId, int statusCode, PurchaseTicket purchaseTicket)
    {
<<<<<<< HEAD
        if (statusCode != Plasma.STATUS_CODE_SUCCESS) {
            Log.v(TAG, "onPurchaseItemInitialized failed");
            threadDelegateOnPurchaseFail();
            threadDecrementTaskCounter();
=======
        // At this point, the purchase ticket is issued but ticket only means
        // that a purchase transaction has initialized successfully, the purchase
        // is not yet complete.
        //
        // If a purchase transaction is successfully initialized, the statusCode
        // of onPurchaseItemInitialized is set as Plasma.STATUS_CODE_SUCCESS.
        // However, if initialization fails, onPurchaseItemFinished will not be
        // called. This is explained in the following section.

        if (statusCode != Plasma.STATUS_CODE_SUCCESS) {
            Log.v(
                TAG,
                "onPurchaseItemInitialized failed for transactionId "
                + transactionId
                + " with status code "
                + statusCode
            );
            pendingTransactions.remove(transactionId);
>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
        }
    }

    @Override
    public void onPurchaseItemFinished(int transactionId, int statusCode, PurchasedItemInformation purchaseItemInformation)
    {
<<<<<<< HEAD
        if (statusCode != Plasma.STATUS_CODE_SUCCESS) {
            Log.v(TAG, "onPurchaseItemFinished failed");
            threadDelegateOnPurchaseFail();
            threadDecrementTaskCounter();
        } else {
            threadDelegateOnItemData(purchaseItemInformation);
            threadDelegateOnPurchaseSucceed(purchaseItemInformation);
            threadDecrementTaskCounter();
=======
        // It is called when a user's purchase transaction is completed, and the
        // final information about purchase transaction is provided.
        //
        // The final information means a receipt of a purchase transaction which
        // is completed.

        pendingTransactions.remove(transactionId);

        if (statusCode == Plasma.STATUS_CODE_SUCCESS) {
            itemDbAdd(purchaseItemInformation.getItemId());
            plasma.requestPurchasedItemInformationList(transactionId++, 1, ITEM_AMOUNT);
        } else {
            Log.v(
                TAG,
                "onPurchaseItemInitialized failed for transactionId "
                + transactionId
                + " with status code "
                + statusCode
            );
>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
        }
    }

    /**
<<<<<<< HEAD
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
=======
     * ---- PLASMA INTERFACE END ----
     */

    private boolean itemDbContains(String productId)
    {
        return ownedItems.contains(productId);
    }

    private void itemDbAdd(String productId)
    {
        ownedItems.add(productId);
    }

    private void itemDbClear()
    {
        SharedPreferences prefs = BBAndroidGame.AndroidGame().GetActivity().getPreferences(Context.MODE_PRIVATE);
        SharedPreferences.Editor edit = prefs.edit();
        edit.clear();
        edit.commit();

        Log.v(TAG, "local item DB cleared");
    }

    private void itemDbSave()
    {
        SharedPreferences prefs = BBAndroidGame.AndroidGame().GetActivity().getPreferences(Context.MODE_PRIVATE);
        SharedPreferences.Editor edit = prefs.edit();
        edit.putString(TAG, getOwnedItemsAsString());
        edit.commit();

        Log.v(TAG, "local item DB saved");
    }

    private void itemDbLoad()
    {
        SharedPreferences prefs = BBAndroidGame.AndroidGame().GetActivity().getPreferences(Context.MODE_PRIVATE);
        try {
            loadOwnedItemsAsString(prefs.getString(TAG, ""));
        } catch (ClassCastException e) {
        }

        Log.v(TAG, "local item DB loaded");
    }

    private void loadOwnedItemsAsString(String input)
    {
        for (String productId : input.split("\n")) {
            ownedItems.add(productId);
        }
    }

    private String getOwnedItemsAsString()
    {
        String result = "";
        for (String item : ownedItems) {
            result += item + "\n";
        }
        return result.substring(0, result.length() - 1);
    }

    /**
     * ---- MONKEY INTERFACE START ----
     */

    public void Init(String itemGroupId)
    {
        if (plasma == null) {
            plasma = new Plasma(itemGroupId, BBAndroidGame.AndroidGame().GetActivity());
            plasma.setPlasmaListener(this);
            itemDbLoad();
        }
    }

    public boolean Purchase(String productId)
    {
        if (plasma == null) {
            Log.v(TAG, "not yet initialized");
            return false;
        }

        if (!plasma.requestPurchaseItem(transactionId++, productId)) {
            Log.v(TAG, "failed to request item purchase");
            return false;
        }

        Log.v(
            TAG,
            "purchase requested for productId " + productId
            + " with transactionId " + (transactionId - 1)
        );
        pendingTransactions.add(transactionId - 1);
        return true;
    }

    public boolean IsBought(String productId)
    {
        return itemDbContains(productId);
    }

    public boolean IsPurchaseInProgress()
    {
        return !pendingTransactions.isEmpty();
>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
    }
}
