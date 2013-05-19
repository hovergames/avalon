package com.avalon.payment;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import java.util.List;

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

        Log.v(TAG, "Creating IAB helper.");
        mHelper = new IabHelper(activity, base64EncodedPublicKey);
        mHelper.enableDebugLogging(true);

        Log.v(TAG, "Starting setup.");
        mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            public void onIabSetupFinished(IabResult result) {
                Log.v(TAG, "Setup finished.");

                if (!result.isSuccess()) {
                    Log.e(TAG, "Problem setting up in-app billing: " + result);
                    return;
                }

                activity.runOnGLThread(new Runnable() {
                    public void run() {
                        Backend.onInitialized();
                    }
                });
            }
        });
    }

    // We're being destroyed. It's important to dispose of the helper here!
    protected void finalize()
    {
        Log.v(TAG, "Destroying helper.");
        if (mHelper != null) mHelper.dispose();
        mHelper = null;
    }

    // Listener that's called when we finish querying the items we own
    final IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener()
    {
        public void onQueryInventoryFinished(IabResult result, final Inventory inventory) {
            Log.v(TAG, "Query inventory finished.");
            if (result.isFailure()) {
                Log.e(TAG, "Failed to query inventory: " + result);
                return;
            }

            if (inventory.getAllDetailsSkus().isEmpty()) {
                Log.e(TAG, "Not a single detail returned. Items defined in the Google Play backend?");
                return;
            }

            for (String sku : inventory.getAllDetailsSkus()) {
                final SkuDetails details = inventory.getSkuDetails(sku);
                activity.runOnGLThread(new Runnable() {
                    public void run() {
                        String title = details.getTitle();

                        // the title contains the name of the app and that's
                        // something we .. well .. don't want to be there.
                        //
                        //     "Iap Title (APP NAME)" ==> "Iap Title"
                        int substrPos = title.lastIndexOf("(") - 1;
                        if (substrPos > 0) {
                            title = title.substring(0, substrPos);
                        }

                        Backend.delegateOnItemData(
                            details.getSku(),
                            title,
                            details.getDescription(),
                            details.getPrice(),
                            0.0f
                        );
                    }
                });
            }

            activity.runOnGLThread(new Runnable() {
                public void run() {
                    Backend.delegateOnServiceStarted();
                }
            });

            for (final String sku : inventory.getAllOwnedSkus()) {
                if (true) {
                    Log.v(TAG, "Consumable item: " + sku + ". Starting consumption.");
                    activity.runOnUiThread(new Runnable() {
                        public void run() {
                            mHelper.consumeAsync(inventory.getPurchase(sku), mConsumeFinishedListener);
                        }
                    });
                } else {
                    Log.v(TAG, "Non-Consumable item " + sku + ". We're done.");
                    activity.runOnGLThread(new Runnable() {
                        public void run() {
                            Backend.delegateOnPurchaseSucceed(sku);
                        }
                    });
                }
            }

            checkTaskCountOnConsumeFinished = true;
        }
    };

    // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener()
    {
        public void onIabPurchaseFinished(IabResult result, final Purchase purchase) {
            Log.v(TAG, "Purchase finished: " + result + ", purchase: " + purchase);

            if (result.isFailure()) {
                Log.e(TAG, "Error purchasing: " + result);
                activity.runOnGLThread(new Runnable() {
                    public void run() {
                        Backend.delegateOnPurchaseFail();
                    }
                });
            } else {
                if (true) {
                    Log.v(TAG, "Consumable item: " + purchase.getSku() + ". Starting consumption.");
                    activity.runOnUiThread(new Runnable() {
                        public void run() {
                            mHelper.consumeAsync(purchase, mConsumeFinishedListener);
                        }
                    });
                    return;
                } else {
                    Log.v(TAG, "Non-Consumable item " + purchase.getSku() + ". We're done.");
                    activity.runOnGLThread(new Runnable() {
                        public void run() {
                            Backend.delegateOnPurchaseSucceed(purchase.getSku());
                        }
                    });
                }
            }

            if (--taskCount == 0) {
                activity.runOnGLThread(new Runnable() {
                    public void run() {
                        Backend.delegateOnTransactionEnd();
                    }
                });
            }
        }
    };

    // Called when consumption is complete
    final IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener()
    {
        public void onConsumeFinished(final Purchase purchase, IabResult result) {
            Log.v(TAG, "Consumption finished. Purchase: " + purchase + ", result: " + result);
            if (!result.isSuccess()) {
                Log.e(TAG, "Error while consuming " + purchase.getSku() + ".");
                return;
            }

            Log.v(TAG, "Consumption of " + purchase.getSku() + " successful. Provisioning.");
            activity.runOnGLThread(new Runnable() {
                public void run() {
                    Backend.delegateOnPurchaseSucceed(purchase.getSku());
                }
            });

            if (checkTaskCountOnConsumeFinished && --taskCount == 0) {
                activity.runOnGLThread(new Runnable() {
                    public void run() {
                        Backend.delegateOnTransactionEnd();
                    }
                });
            }
        }
    };

    /**
     *
     * PUBLIC API FOR com.avalon.payment.Backend
     *
     */

    public void purchase(String sku)
    {
        if (++taskCount == 1) {
            activity.runOnGLThread(new Runnable() {
                public void run() {
                    Backend.delegateOnTransactionStart();
                }
            });
        }

        Log.v(TAG, "Purchase started for: " + sku);
        mHelper.launchPurchaseFlow(activity, sku, RC_REQUEST, mPurchaseFinishedListener);
    }

    public void startItemDataRequest(final List<String> moreSkus)
    {
        Log.v(TAG, "Setup successful. Querying inventory.");
        activity.runOnUiThread(new Runnable() {
            public void run() {
                mHelper.queryInventoryAsync(true, moreSkus, mGotInventoryListener);
            }
        });
    }

    public boolean handleActivityResult(int requestCode, int resultCode, Intent data)
    {
        Log.v(TAG, "handleActivityResult(" + requestCode + "," + resultCode + "," + data);
        return mHelper.handleActivityResult(requestCode, resultCode, data);
    }
}
