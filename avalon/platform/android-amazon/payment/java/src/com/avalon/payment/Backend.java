package com.avalon.payment;

import java.util.HashSet;
import com.avalon.payment.PurchasingObserver;

public class Backend
{
    private static PurchasingObserver mPurchaseObserver = null;
    private static HashSet<String> pendingItemData = new HashSet<String>();
    private static int itemDataReturned = 0;

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

    public static void purchase(String productId)
    {
        mPurchaseObserver.purchase(productId);
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
        pendingItemData.clear();
    }

    public static void delegateOnItemData(String productId, String name, String desc, String priceStr, float price)
    {
        ++itemDataReturned;
        onItemData(productId, name, desc, priceStr, price);
    }

    public static native void delegateOnServiceStarted();
    public static native void delegateOnPurchaseSucceed(String productId);
    public static native void delegateOnPurchaseFail();
    public static native void delegateOnTransactionStart();
    public static native void delegateOnTransactionEnd();
    public static native void onItemData(String productId, String name, String desc, String priceStr, float price);
    public static native void onInitialized();
}
