package com.avalon.payment;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

import com.example.android.trivialdrivesample.util.IabHelper;
import com.example.android.trivialdrivesample.util.IabResult;
import com.example.android.trivialdrivesample.util.Inventory;
import com.example.android.trivialdrivesample.util.Purchase;
import com.example.android.trivialdrivesample.util.SkuDetails;

import org.cocos2dx.lib.Cocos2dxActivity;
import com.avalon.payment.Backend;

public class PurchasingObserver
{
    static final String TAG = "avalon.payment.PurchasingObserver";
    static final int RC_REQUEST = 10001;
    Cocos2dxActivity activity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
    public static String base64EncodedPublicKey;
    IabHelper mHelper;
    private Map<String, Boolean> productIds;
    private Integer taskCount = 0;
    boolean checkTaskCountOnConsumeFinished = false;

    public PurchasingObserver()
    {
        if (base64EncodedPublicKey.isEmpty()) {
            activity.runOnGLThread(new Runnable() {
                public void run() {
                    throw new RuntimeException("Required public key not set!");
                }
            });
        }

        mHelper = new IabHelper(activity, base64EncodedPublicKey);
        mHelper.enableDebugLogging(false);
        mHelper.startSetup(mSetupFinishedListener);
    }

    protected void finalize()
    {
        if (mHelper != null) {
            mHelper.dispose();
        }
        mHelper = null;
    }

    private boolean isConsumable(String sku)
    {
        return (productIds.containsKey(sku) && productIds.get(sku));
    }

    private String clearTitle(String title)
    {
        // "Iap Title (APP NAME)" ==> "Iap Title"
        int substrPos = title.lastIndexOf("(") - 1;
        if (substrPos > 0) {
            title = title.substring(0, substrPos);
        }
        return title;
    }

    /**
     *
     * Async callback methods
     *
     */

    final IabHelper.OnIabSetupFinishedListener mSetupFinishedListener = new IabHelper.OnIabSetupFinishedListener()
    {
        public void onIabSetupFinished(IabResult result) {
            if (!result.isSuccess()) {
                Log.e(TAG, "onIabSetupFinished failed: " + result);
                return;
            }

            threadOnInitialized();
        }
    };

    final IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener()
    {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            if (result.isFailure()) {
                Log.e(TAG, "onQueryInventoryFinished failed: " + result);
                return;
            }

            if (inventory.getAllDetailsSkus().isEmpty()) {
                Log.e(TAG, "onQueryInventoryFinished failed: Not a single detail returned! Google Play configured?");
                return;
            }

            threadDelegateItemData(inventory);
            threadDelegateOnServiceStarted();

            for (String sku : inventory.getAllOwnedSkus()) {
                if (isConsumable(sku)) {
                    threadConsumeAsync(inventory.getPurchase(sku));
                } else {
                    threadDelegateOnPurchaseSucceed(sku);
                }
            }

            checkTaskCountOnConsumeFinished = true;
        }
    };

    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener()
    {
        public void onIabPurchaseFinished(IabResult result, final Purchase purchase) {
            if (result.isFailure()) {
                Log.e(TAG, "onIabPurchaseFinished failed: " + result);
                threadDelegateOnPurchaseFail();
            } else if (isConsumable(purchase.getSku())) {
                threadConsumeAsync(purchase);
            } else {
                threadDelegateOnPurchaseSucceed(purchase.getSku());
            }

            threadDecrementTaskCounter();
        }
    };

    final IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener()
    {
        public void onConsumeFinished(final Purchase purchase, IabResult result) {
            if (!result.isSuccess()) {
                Log.e(TAG, "onConsumeFinished failed: " + result);
                return;
            }

            threadDelegateOnPurchaseSucceed(purchase.getSku());
            if (checkTaskCountOnConsumeFinished) {
                threadDecrementTaskCounter();
            }
        }
    };

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

    private void threadDelegateOnPurchaseFail()
    {
        activity.runOnGLThread(new Runnable() {
            public void run() {
                Backend.delegateOnPurchaseFail();
            }
        });
    }

    private void threadConsumeAsync(final Purchase purchase)
    {
        activity.runOnUiThread(new Runnable() {
            public void run() {
                mHelper.consumeAsync(purchase, mConsumeFinishedListener);
            }
        });
    }

    private void threadDelegateOnPurchaseSucceed(final String sku)
    {
        activity.runOnGLThread(new Runnable() {
            public void run() {
                Backend.delegateOnPurchaseSucceed(sku);
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

    private void threadDelegateItemData(Inventory inventory)
    {
        for (String sku : inventory.getAllDetailsSkus()) {
            final SkuDetails details = inventory.getSkuDetails(sku);

            activity.runOnGLThread(new Runnable() {
                public void run() {
                    Backend.delegateOnItemData(
                        details.getSku(),
                        clearTitle(details.getTitle()),
                        details.getDescription(),
                        details.getPrice(),
                        0.0f
                    );
                }
            });
        }
    }

    /**
     *
     * PUBLIC API FOR com.avalon.payment.Backend
     *
     */

    public void purchase(String sku, boolean isConsumable)
    {
        threadIncrementTaskCounter();
        mHelper.launchPurchaseFlow(activity, sku, RC_REQUEST, mPurchaseFinishedListener);
    }

    public void startItemDataRequest(final Map<String, Boolean> productIds)
    {
        this.productIds = productIds;
        final List<String> moreSkus = new ArrayList<String>(productIds.keySet());

        activity.runOnUiThread(new Runnable() {
            public void run() {
                mHelper.queryInventoryAsync(true, moreSkus, mGotInventoryListener);
            }
        });
    }

    public boolean handleActivityResult(int requestCode, int resultCode, Intent data)
    {
        return mHelper.handleActivityResult(requestCode, resultCode, data);
    }
}
