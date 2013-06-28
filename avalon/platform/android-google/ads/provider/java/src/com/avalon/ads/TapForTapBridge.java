package com.avalon.ads;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.Activity;
import android.widget.RelativeLayout;
import android.widget.FrameLayout;
import android.view.Gravity;

import com.tapfortap.TapForTap;
import com.tapfortap.AdView;
import com.tapfortap.Interstitial;

abstract class TapForTapBridge
{
    static Cocos2dxActivity activity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
    static RelativeLayout adView = null;

    public static void init(String id)
    {
        TapForTap.initialize(activity, id);
        Interstitial.prepare(activity);
    }

    public static void showFullscreenAd()
    {
        Interstitial.show(activity);
    }

    public static void showBanner()
    {
        if (adView != null) {
            return;
        }
        
        adView = new RelativeLayout(activity);
        adView.setGravity(Gravity.BOTTOM);
        adView.addView(new AdView(activity));
        
        activity.runOnUiThread(new Runnable() {
            public void run() {
                FrameLayout mainLayout = (FrameLayout) activity.findViewById(android.R.id.content);
                mainLayout.addView(adView);
            }
        });
    }

    public static void hideAds()
    {
        if (adView == null) {
            return;
        }

        activity.runOnUiThread(new Runnable() {
            public void run() {
                FrameLayout mainLayout = (FrameLayout) activity.findViewById(android.R.id.content);
                mainLayout.removeView(adView);

                adView.removeAllViews();
                adView = null;
            }
        });
    }
}