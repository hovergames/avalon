package com.avalon.payment;

import com.samsungapps.plasma.*;

public class PurchasingObserver implements PlasmaListener
{
    private static final int ITEM_AMOUNT = 25;
    private static final String TAG = "avalon.payment.PurchasingObserver";
    private Plasma plasma = null;
    private int transactionId = 0;
    private Set<Integer> pendingTransactions = new HashSet<Integer>();
    private Set<String> ownedItems = new HashSet<String>();

    /**
     * ---- PLASMA INTERFACE START ----
     */

    @Override
    public void onItemInformationListReceived(int transactionId, int statusCode, ArrayList<ItemInformation> itemInformationList)
    {
        // Not used as we don't expose the abillity to list / show all available
        // items in out itemGroup.
    }

    @Override
    public void onPurchasedItemInformationListReceived(int transactionId, int statusCode, ArrayList<PurchasedItemInformation> purchasedItemInformationList)
    {
        // Contains a list of all purchased items in our itemGroup. Triggered
        // via requestPurchasedItemInformationList() after each complete
        // purchase to keep the local item DB in sync.

        itemDbClear();
        for (PurchasedItemInformation itemInformation : purchasedItemInformationList) {
            itemDbAdd(itemInformation.getItemId());
        }
        itemDbSave();
    }

    @Override
    public void onPurchaseItemInitialized(int transactionId, int statusCode, PurchaseTicket purchaseTicket)
    {
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
        }
    }

    @Override
    public void onPurchaseItemFinished(int transactionId, int statusCode, PurchasedItemInformation purchaseItemInformation)
    {
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
        }
    }

    /**
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
    }
}
