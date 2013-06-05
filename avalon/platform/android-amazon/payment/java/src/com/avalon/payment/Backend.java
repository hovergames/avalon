package com.avalon.payment;

import java.util.Map;
import java.util.HashMap;

import com.avalon.payment.PurchasingObserver;

public class Backend
{
    private static PurchasingObserver mPurchaseObserver = null;
    private static Map<String, Boolean> pendingItemData = new HashMap<String, Boolean>();
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

    public static void addItemDataRequest(String productId, boolean isConsumable)
    {
        pendingItemData.put(productId, isConsumable);
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
}
