package com.avalon.payment;

/**
 * Open TODO's
 * ===========
 *
 * - (#REF1) We should send all stored SKU's to the backend. Why? Because
 *           this is the way sync already purchased items with the app! It
 *           should be safe to simply forward all stored SKU's, as we don't keep
 *           track of consumables (that's the job of the app anyway).
 *
 * - (#REF2) The whole "user has changed!"-handling is more or less untested.
 *           We should, IMHO, clear the local store of SKU'si, if we detect a
 *           missmatch. But is it safe to clear the whole shared preferences?
 *           Isn't it the same backend as in CCUserDefaults?
 *
 * - Check if both supported item types (consumable and non-consumable) work
 *   as expected. This code is simply copied from Monkey/bono and trimmed here
 *   and there. It looked good in a first rough test but this should be
 *   verified with more care. Hint: Please test a unstable / lost internet
 *   connection too!
 */

import android.content.Context;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.util.Log;

import com.amazon.inapp.purchasing.BasePurchasingObserver;
import com.amazon.inapp.purchasing.GetUserIdResponse;
import com.amazon.inapp.purchasing.GetUserIdResponse.GetUserIdRequestStatus;
import com.amazon.inapp.purchasing.Offset;
import com.amazon.inapp.purchasing.Item.ItemType;
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
    public Integer taskCount = 0;

    public PurchasingObserver()
    {
        super(Cocos2dxActivity.getContext());
        PurchasingManager.registerObserver(this);
    }

    public void purchase(String productId)
    {
        Log.v(TAG, "purchase started: SKU - " + productId);
        final String requestId = PurchasingManager.initiatePurchaseRequest(productId);
        Log.v(TAG, "RequestId: " + requestId);

        if (++taskCount == 1) {
            Backend.delegateOnTransactionStart();
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
        Log.v(TAG, "RequestId: " + getUserIdResponse.getRequestId());
        Log.v(TAG, "UserIdRequestStatus: " + getUserIdResponse.getUserIdRequestStatus());
        new GetUserIdAsyncTask().execute(getUserIdResponse);
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
        Log.v(TAG, "PurchaseRequestStatus: " + purchaseResponse.getPurchaseRequestStatus());
        Log.v(TAG, "RequestID: " + purchaseResponse.getRequestId());
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
        Log.v(TAG, "onPurchaseUpdatesRecived recieved: Response -" + purchaseUpdatesResponse);
        Log.v(TAG, "PurchaseUpdatesRequestStatus: " + purchaseUpdatesResponse.getPurchaseUpdatesRequestStatus());
        Log.v(TAG, "RequestID: " + purchaseUpdatesResponse.getRequestId());
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
                Backend.delegateOnServiceStarted();

                // TODO (#REF1): See file header

                // Call initiatePurchaseUpdatesRequest for the returned user
                // to sync purchases that are not yet fulfilled.
                final SharedPreferences settings = getSharedPreferencesForCurrentUser();
                PurchasingManager.initiatePurchaseUpdatesRequest(Offset.fromString(settings.getString(OFFSET, Offset.BEGINNING.toString())));
            }
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
            final SharedPreferences settings = getSharedPreferencesForCurrentUser();
            final SharedPreferences.Editor editor = settings.edit();

            // currently logged in user is different than what we have so
            // update the state
            if (!purchaseResponse.getUserId().equals(userId)) {
                userId = purchaseResponse.getUserId();
                // TODO (#REF2): editor.clear(); editor.commit(); ??
                PurchasingManager.initiatePurchaseUpdatesRequest(Offset.BEGINNING);
                return false;
            }

            switch (purchaseResponse.getPurchaseRequestStatus()) {
            case SUCCESSFUL:
            case ALREADY_ENTITLED:
                final Receipt receipt = purchaseResponse.getReceipt();
                printReceipt(receipt);

                Backend.delegateOnPurchaseSucceed(receipt.getSku());
                if (receipt.getItemType() == ItemType.ENTITLED) {
                    editor.putBoolean(receipt.getSku(), true);
                    editor.commit();
                }
                return true;

            case FAILED:
                Backend.delegateOnPurchaseFail();
                Log.v(TAG, "Failed purchase for requestId: " + purchaseResponse.getRequestId());
                return false;

            case INVALID_SKU:
                Backend.delegateOnPurchaseFail();
                Log.v(TAG, "Invalid SKU for requestId: " + purchaseResponse.getRequestId());
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
    private class PurchaseUpdatesAsyncTask extends AsyncTask<PurchaseUpdatesResponse, Void, Boolean>
    {
        @Override
        protected Boolean doInBackground(final PurchaseUpdatesResponse... params)
        {
            final PurchaseUpdatesResponse purchaseUpdatesResponse = params[0];
            final SharedPreferences prefs = getSharedPreferencesForCurrentUser();
            final SharedPreferences.Editor editor = prefs.edit();

            // currently logged in user is different than what we have so update the state
            if (!purchaseUpdatesResponse.getUserId().equals(userId)) {
                return false;
            }

            for (final String sku : purchaseUpdatesResponse.getRevokedSkus()) {
                Log.v(TAG, "Revoked SKU: " + sku);
                editor.putBoolean(sku, false);
                editor.commit();
            }

            if (purchaseUpdatesResponse.getPurchaseUpdatesRequestStatus() == PurchaseUpdatesRequestStatus.SUCCESSFUL) {
                for (final Receipt receipt : purchaseUpdatesResponse.getReceipts()) {
                    printReceipt(receipt);
                    if (receipt.getItemType() == ItemType.ENTITLED) {
                        Backend.delegateOnPurchaseSucceed(receipt.getSku());
                        editor.putBoolean(receipt.getSku(), true);
                        editor.commit();
                    }
                }
            }

            return false;
        }

        @Override
        protected void onPostExecute(final Boolean success)
        {
            super.onPostExecute(success);
        }
    }
}
