package com.avalon.payment;

import com.avalon.payment.PurchasingObserver;

public class Backend
{
    private static PurchasingObserver mPurchaseObserver = null;

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
        return isInitialized();
    }

    public static native void delegateOnServiceStarted();
    public static native void delegateOnPurchaseSucceed(String productId);
    public static native void delegateOnPurchaseFail();
    public static native void delegateOnTransactionStart();
    public static native void delegateOnTransactionEnd();
}
