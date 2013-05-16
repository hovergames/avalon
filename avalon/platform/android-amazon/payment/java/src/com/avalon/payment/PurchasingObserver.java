package com.avalon.payment;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.util.Log;

import java.util.HashSet;
import java.util.Map;
import java.util.HashMap;

import com.amazon.inapp.purchasing.BasePurchasingObserver;
import com.amazon.inapp.purchasing.GetUserIdResponse;
import com.amazon.inapp.purchasing.GetUserIdResponse.GetUserIdRequestStatus;
import com.amazon.inapp.purchasing.Offset;
import com.amazon.inapp.purchasing.Item;
import com.amazon.inapp.purchasing.Item.ItemType;
import com.amazon.inapp.purchasing.ItemDataResponse;
import com.amazon.inapp.purchasing.PurchaseResponse;
import com.amazon.inapp.purchasing.PurchaseUpdatesResponse;
import com.amazon.inapp.purchasing.PurchaseUpdatesResponse.PurchaseUpdatesRequestStatus;
import com.amazon.inapp.purchasing.PurchasingManager;
import com.amazon.inapp.purchasing.Receipt;

import org.cocos2dx.lib.Cocos2dxActivity;
import com.avalon.payment.Backend;

/**
 * Purchasing Observer will be called on by the Purchasing Manager asynchronously.
 * Since the methods on the UI thread of the application, all fulfillment logic
 * is done via an AsyncTask. This way, any intensive processes will not hang
 * the UI thread and cause the application to become unresponsive.
 */
public class PurchasingObserver extends BasePurchasingObserver
{
    private static final String OFFSET = "offset";
    private static final String TAG = "avalon.payment.PurchasingObserver";
    private String userId;
    public Map<String, String> requestIds;
    public Integer taskCount = 0;

    /**
     * Creates new instance of the PurchasingObserver class.
     */
    public PurchasingObserver()
    {
        super(Cocos2dxActivity.getContext());
        PurchasingManager.registerObserver(this);
        requestIds = new HashMap<String, String>();
    }

    /**
     * Interface method for com.avalon.payment.Backend
     *
     * @param productId
     */
    public void purchase(String productId)
    {
        Log.v(TAG, "purchase started: SKU - " + productId);
        final String requestId = PurchasingManager.initiatePurchaseRequest(productId);
        requestIds.put(requestId, productId);

        if (++taskCount == 1) {
            Backend.delegateOnTransactionStart();
        }
    }

    /**
     * Inteface method for com.avalon.payment.Backend
     *
     * @param productIds
     */
    public void startItemDataRequest(HashSet<String> productIds)
    {
        if (productIds.isEmpty()) {
            sendStoresProductsAndStartService();
        } else {
            Log.v(TAG, "initiateItemDataRequest");
            PurchasingManager.initiateItemDataRequest(productIds);
        }
    }

    /**
     * Invoked once the observer is registered with the Puchasing Manager if
     * the boolean is false, the application is receiving responses from the
     * SDK Tester. If the boolean is true, the application is live in production.
     *
     * @param isSandboxMode
     *            Boolean value that shows if the app is live or not.
     */
    @Override
    public void onSdkAvailable(final boolean isSandboxMode)
    {
        Log.v(TAG, "onSdkAvailable recieved: Response - " + isSandboxMode);
        PurchasingManager.initiateGetUserIdRequest();
    }

    /**
     * Invoked once the call from initiateGetUserIdRequest is completed.
     * On a successful response, a response object is passed which contains the
     * request id, request status, and the userid generated for your application.
     *
     * @param getUserIdResponse
     *            Response object containing the UserID
     */
    @Override
    public void onGetUserIdResponse(final GetUserIdResponse getUserIdResponse)
    {
        Log.v(TAG, "onGetUserIdResponse recieved: Response - " + getUserIdResponse);
        new GetUserIdAsyncTask().execute(getUserIdResponse);
    }

    /**
     * Invoked once the call from initiateItemDataRequest is completed.
     * On a successful response, a response object is passed which contains the request id, request status, and a set of
     * item data for the requested skus. Items that have been suppressed or are unavailable will be returned in a
     * set of unavailable skus.
     *
     * @param itemDataResponse
     *            Response object containing a set of purchasable/non-purchasable items
     */
    @Override
    public void onItemDataResponse(final ItemDataResponse itemDataResponse) {
        Log.v(TAG, "onItemDataResponse recieved: Response - " + itemDataResponse);
        new ItemDataAsyncTask().execute(itemDataResponse);
    }

    /**
     * Is invoked once the call from initiatePurchaseRequest is completed.
     * On a successful response, a response object is passed which contains the
     * request id, request status, and the receipt of the purchase.
     *
     * @param purchaseResponse
     *            Response object containing a receipt of a purchase
     */
    @Override
    public void onPurchaseResponse(final PurchaseResponse purchaseResponse)
    {
        Log.v(TAG, "onPurchaseResponse recieved: Response - " + purchaseResponse);
        new PurchaseAsyncTask().execute(purchaseResponse);
    }

    /**
     * Is invoked once the call from initiatePurchaseUpdatesRequest is completed.
     * On a successful response, a response object is passed which contains the
     * request id, request status, a set of previously purchased receipts, a set
     * of revoked skus, and the next offset if applicable. If a user downloads
     * your application to another device, this call is used to sync up this
     * device with all the user's purchases.
     *
     * @param purchaseUpdatesResponse
     *            Response object containing the user's recent purchases.
     */
    @Override
    public void onPurchaseUpdatesResponse(final PurchaseUpdatesResponse purchaseUpdatesResponse)
    {
        Log.v(TAG, "onPurchaseUpdatesResponse recieved: Response -" + purchaseUpdatesResponse);
        new PurchaseUpdatesAsyncTask().execute(purchaseUpdatesResponse);
    }

    /*
     * Helper method to print out relevant receipt information to the log.
     */
    private void printReceipt(final Receipt receipt)
    {
        Log.v(TAG, String.format(
            "Receipt: ItemType: %s Sku: %s SubscriptionPeriod: %s",
            receipt.getItemType(),
            receipt.getSku(),
            receipt.getSubscriptionPeriod()
        ));
    }

    /*
     * Helper method to access the users stores settings
     */
    public SharedPreferences getSharedPreferencesForCurrentUser()
    {
        return Cocos2dxActivity.getContext().getSharedPreferences(userId, Context.MODE_PRIVATE);
    }

    /*
     * Helper method that sends all stored SKUs for the current user to the
     * backend and - after that's done - flag the service as started.
     */
    public void sendStoresProductsAndStartService()
    {
        Backend.delegateOnServiceStarted();
        for (Map.Entry<String,?> entry : getSharedPreferencesForCurrentUser().getAll().entrySet()) {
            if (entry.getKey().equals(OFFSET)) {
                continue;
            }
            Backend.delegateOnPurchaseSucceed(entry.getKey());
        }
    }

    /*
     * Started when the Observer receives a GetUserIdResponse. The Shared
     * Preferences file for the returned user id is accessed.
     */
    private class GetUserIdAsyncTask extends AsyncTask<GetUserIdResponse, Void, Boolean>
    {
        @Override
        protected Boolean doInBackground(final GetUserIdResponse... params)
        {
            GetUserIdResponse getUserIdResponse = params[0];

            if (getUserIdResponse.getUserIdRequestStatus() == GetUserIdRequestStatus.SUCCESSFUL) {
                userId = getUserIdResponse.getUserId();
                return true;
            } else {
                Log.v(TAG, "onGetUserIdResponse: Unable to get user ID.");
                return false;
            }
        }

        @Override
        protected void onPostExecute(final Boolean result)
        {
            super.onPostExecute(result);

            if (result) {
                Backend.onInitialized();

                // Call initiatePurchaseUpdatesRequest for the returned user
                // to sync purchases that are not yet fulfilled.
                final SharedPreferences settings = getSharedPreferencesForCurrentUser();
                PurchasingManager.initiatePurchaseUpdatesRequest(Offset.fromString(
                    settings.getString(OFFSET, Offset.BEGINNING.toString())
                ));
            }
        }
    }

    /*
     * Started when the observer receives an Item Data Response.
     * Takes the items and display them in the logs. You can use this
     * information to display an in game storefront for your IAP items.
     */
    private class ItemDataAsyncTask extends AsyncTask<ItemDataResponse, Void, Void> {
        @Override
        protected Void doInBackground(final ItemDataResponse... params) {
            final ItemDataResponse itemDataResponse = params[0];

            switch (itemDataResponse.getItemDataRequestStatus()) {
                case SUCCESSFUL_WITH_UNAVAILABLE_SKUS:
                    // Skus that you can not purchase will be here.
                    for (final String s : itemDataResponse.getUnavailableSkus()) {
                        Log.v(TAG, "onItemDataResponse: Unavailable SKU: " + s);
                        Backend.onItemData(s, "", "", "", 0.0f);
                    }

                case SUCCESSFUL:
                    // Information you'll want to display about your IAP items is here
                    // In this example we'll simply log them.
                    final Map<String, Item> items = itemDataResponse.getItemData();
                    for (final String key : items.keySet()) {
                        Item i = items.get(key);
                        Backend.onItemData(key, i.getTitle(), i.getDescription(), i.getPrice(), 0.0f);
                        Log.v(TAG, String.format(
                            "onItemDataResponse: Item\n  Title: %s\n  Type: %s\n  SKU: %s\n  Price: %s\n  Description: %s\n",
                            i.getTitle(), i.getItemType(), i.getSku(), i.getPrice(), i.getDescription()
                        ));
                    }
                    break;

                case FAILED:
                    // On failed responses will fail gracefully.
                    break;
            }

            sendStoresProductsAndStartService();
            return null;
        }
    }

    /*
     * Started when the observer receives a Purchase Response
     * Once the AsyncTask returns successfully, the UI is updated.
     */
    private class PurchaseAsyncTask extends AsyncTask<PurchaseResponse, Void, Boolean>
    {
        @Override
        protected Boolean doInBackground(final PurchaseResponse... params)
        {
            final PurchaseResponse purchaseResponse = params[0];

            // currently logged in user is different than what we have so update the state
            if (!purchaseResponse.getUserId().equals(userId)) {
                userId = purchaseResponse.getUserId();
                final SharedPreferences settings = getSharedPreferencesForCurrentUser();
                PurchasingManager.initiatePurchaseUpdatesRequest(Offset.fromString(
                    settings.getString(OFFSET, Offset.BEGINNING.toString())
                ));
                return false;
            }

            final SharedPreferences settings = getSharedPreferencesForCurrentUser();
            final SharedPreferences.Editor editor = settings.edit();
            String sku;

            switch (purchaseResponse.getPurchaseRequestStatus()) {
                case SUCCESSFUL:
                    final Receipt receipt = purchaseResponse.getReceipt();
                    printReceipt(receipt);

                    Backend.delegateOnPurchaseSucceed(receipt.getSku());
                    if (receipt.getItemType() == ItemType.ENTITLED) {
                        editor.putBoolean(receipt.getSku(), true);
                        editor.commit();
                    }
                    return true;

                case ALREADY_ENTITLED:
                    sku = requestIds.get(purchaseResponse.getRequestId());
                    requestIds.remove(purchaseResponse.getRequestId());

                    Backend.delegateOnPurchaseSucceed(sku);
                    editor.putBoolean(sku, true);
                    editor.commit();
                    return true;

                case FAILED:
                    sku = requestIds.get(purchaseResponse.getRequestId());
                    requestIds.remove(purchaseResponse.getRequestId());

                    Backend.delegateOnPurchaseFail();
                    Log.v(TAG, "onPurchaseResponse: Failed SKU: " + sku);
                    return false;

                case INVALID_SKU:
                    sku = requestIds.get(purchaseResponse.getRequestId());
                    requestIds.remove(purchaseResponse.getRequestId());
                    
                    Backend.delegateOnPurchaseFail();
                    Log.v(TAG, "onPurchaseResponse: Invalid SKU: " + sku);
                    return false;
            }

            return false;
        }

        @Override
        protected void onPostExecute(final Boolean success)
        {
            super.onPostExecute(success);
            if (--taskCount == 0) {
                Backend.delegateOnTransactionEnd();
            }
        }
    }

    /*
     * Started when the observer receives a Purchase Updates Response. Once the
     * AsyncTask returns successfully, we'll update the UI.
     */
    private class PurchaseUpdatesAsyncTask extends AsyncTask<PurchaseUpdatesResponse, Void, Void>
    {
        @Override
        protected Void doInBackground(final PurchaseUpdatesResponse... params)
        {
            final PurchaseUpdatesResponse purchaseUpdatesResponse = params[0];

            // currently logged in user is different than what we have so update the state
            if (!purchaseUpdatesResponse.getUserId().equals(userId)) {
                return null;
            }

            final SharedPreferences prefs = getSharedPreferencesForCurrentUser();
            final SharedPreferences.Editor editor = prefs.edit();

            /*
             * If the customer for some reason had items revoked, the skus for these items will be contained in the
             * revoked skus set.
             */
            for (final String sku : purchaseUpdatesResponse.getRevokedSkus()) {
                Log.v(TAG, "onPurchaseUpdatesResponse: Revoked SKU: " + sku);
                editor.remove(sku);
                editor.commit();
            }

            /*
             * On failed responses the application will ignore the request.
             */
            if (purchaseUpdatesResponse.getPurchaseUpdatesRequestStatus() == PurchaseUpdatesRequestStatus.FAILED) {
            	return null;
            }

            for (final Receipt receipt : purchaseUpdatesResponse.getReceipts()) {
                printReceipt(receipt);

                if (receipt.getItemType() == ItemType.ENTITLED) {
                    Backend.delegateOnPurchaseSucceed(receipt.getSku());
                    editor.putBoolean(receipt.getSku(), true);
                    editor.commit();
                }
            }
            
            /*
             * Store the offset into shared preferences. If there has been more purchases since the
             * last time our application updated, another initiatePurchaseUpdatesRequest is called with the new
             * offset.
             */
            final Offset newOffset = purchaseUpdatesResponse.getOffset();
            editor.putString(OFFSET, newOffset.toString());
            editor.commit();
            if (purchaseUpdatesResponse.isMore()) {
                Log.v(TAG, "onPurchaseUpdatesResponse: Initiating Another Purchase Updates with offset: " + newOffset.toString());
                PurchasingManager.initiatePurchaseUpdatesRequest(newOffset);
            }

            return null;
        }
    }
}
