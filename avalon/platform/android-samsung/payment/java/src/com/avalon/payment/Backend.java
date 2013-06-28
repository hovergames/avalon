package com.avalon.payment;

<<<<<<< HEAD
import java.util.Map;
import java.util.HashMap;
=======
import java.util.List;
import java.util.ArrayList;
import android.content.Intent;
>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052

import com.avalon.payment.PurchasingObserver;

public class Backend
{
    private static PurchasingObserver mPurchaseObserver = null;
<<<<<<< HEAD
    private static Map<String, Boolean> pendingItemData = new HashMap<String, Boolean>();
=======
    private static List<String> pendingItemData = new ArrayList<String>();
>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
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

<<<<<<< HEAD
    public static void addItemDataRequest(String productId, boolean isConsumable)
    {
        pendingItemData.put(productId, isConsumable);
=======
    public static void addItemDataRequest(String productId)
    {
        pendingItemData.add(productId);
>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
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

<<<<<<< HEAD
=======

>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
    /**
     *
     * Helper methods to integrate this lib into your app. Should be called in
     * your main activity - please read the docs provided with this library!
     *
     */

<<<<<<< HEAD
    public static void setItemGroupId(final String itemGroupId)
    {
        PurchasingObserver.itemGroupId = itemGroupId;
=======
    public static boolean handleActivityResult(int requestCode, int resultCode, Intent data)
    {
        initialize();
        return mPurchaseObserver.handleActivityResult(requestCode, resultCode, data);
    }

    public static void setPublicKey(final String publicKey)
    {
        PurchasingObserver.base64EncodedPublicKey = publicKey;
>>>>>>> ef2028816c3a5f5dd5ccb08d8f7c4832134e0052
    }
}
