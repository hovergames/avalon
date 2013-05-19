package com.avalon.payment;

import java.util.List;
import java.util.ArrayList;
import android.content.Intent;

import com.avalon.payment.PurchasingObserver;

public class Backend
{
    private static PurchasingObserver mPurchaseObserver = null;
    private static List<String> pendingItemData = new ArrayList<String>();
    private static int itemDataReturned = 0;

    /**
     *
     * Methods called from the C++ side
     *
     */

    public static boolean isInitialized()
    {
        return (mPurchaseObserver != null);
    }

    public static void initialize()
    {
        if (!isInitialized()) {
            mPurchaseObserver = new PurchasingObserver();
        }
    }

    public static void shutdown()
    {
    }

    public static void purchase(String productId, boolean isConsumable)
    {
        mPurchaseObserver.purchase(productId, isConsumable);
    }

    public static boolean isPurchaseReady()
    {
        return isInitialized() && itemDataReturned > 0;
    }

    public static void addItemDataRequest(String productId)
    {
        pendingItemData.add(productId);
    }

    public static void startItemDataRequest()
    {
        mPurchaseObserver.startItemDataRequest(pendingItemData);
    }

    public static void delegateOnItemData(String productId, String name, String desc, String priceStr, float price)
    {
        pendingItemData.clear();
        ++itemDataReturned;
        onItemData(productId, name, desc, priceStr, price);
    }

    /**
     *
     * Methods to call back into the C++ side
     *
     */

    public static native void delegateOnServiceStarted();
    public static native void delegateOnPurchaseSucceed(String productId);
    public static native void delegateOnPurchaseFail();
    public static native void delegateOnTransactionStart();
    public static native void delegateOnTransactionEnd();
    public static native void onItemData(String productId, String name, String desc, String priceStr, float price);
    public static native void onInitialized();


    /**
     *
     * Helper methods to integrate this lib into your app. Should be called in
     * your main activity - please read the docs provided with this library!
     *
     */

    public static boolean handleActivityResult(int requestCode, int resultCode, Intent data)
    {
        initialize();
        return mPurchaseObserver.handleActivityResult(requestCode, resultCode, data);
    }

    public static void setPublicKey(final String publicKey)
    {
        PurchasingObserver.base64EncodedPublicKey = publicKey;
    }
}
