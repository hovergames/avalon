package com.avalon.ui;

import java.util.Map;
import java.util.HashMap;
import android.util.Log;

import android.app.AlertDialog;
import android.content.DialogInterface;

import org.cocos2dx.lib.Cocos2dxHelper;
import android.app.Activity;

class OnClick implements DialogInterface.OnClickListener
{
    private long delegatePtr;
    private int index;
    private String label;

    public OnClick(long delegatePtr, int index, String label)
    {
        this.delegatePtr = delegatePtr;
        this.index = index;
        this.label = label;
    }

    public void onClick(DialogInterface dialog, int id)
    {
        Cocos2dxHelper.runOnGLThread(new Runnable() {
            public void run() {
                Alert.onClick(delegatePtr, index, label);
            }
        });
    }
}

class Alert
{
    private static HashMap<Integer, String> buttons = new HashMap<Integer, String>();
    private static final String TAG = "avalon.ui.Alert";

    public static void reset()
    {
        buttons.clear();
    }

    public static void addButton(int index, String label)
    {
        buttons.put(index, label);
    }

    public static void show(final String title, final String message, final long delegatePtr)
    {
        Cocos2dxHelper.getActivity().runOnUiThread(new Runnable() {
            public void run() {
                AlertDialog.Builder alert = new AlertDialog.Builder(Cocos2dxHelper.getActivity());
                alert.setTitle(title);
                alert.setMessage(message);

                int counter = 0;
                for (Map.Entry<Integer, String> entry : buttons.entrySet()) {
                    ++counter;
                    OnClick callback = new OnClick(delegatePtr, entry.getKey(), entry.getValue());

                    if (counter == 1) {
                        alert.setNegativeButton(entry.getValue(), callback);
                    } else if (counter == 2) {
                        alert.setNeutralButton(entry.getValue(), callback);
                    } else if (counter == 3) {
                        alert.setPositiveButton(entry.getValue(), callback);
                    } else {
                        Log.v(TAG, "Button '" + entry.getValue() + "' with index " + entry.getKey() + " ignored!");
                    }
                }

                alert.create().show();
                reset();
            }
        });

    }

    public static native void onClick(long delegatePtr, int index, String label);
}