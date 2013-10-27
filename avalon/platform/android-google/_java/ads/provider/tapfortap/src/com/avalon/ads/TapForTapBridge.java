package com.avalon.ads;

import org.cocos2dx.lib.Cocos2dxHelper;

import android.util.Log;
import android.app.Activity;
import android.widget.RelativeLayout;
import android.widget.FrameLayout;
import android.view.Gravity;

import com.tapfortap.TapForTap;
import com.tapfortap.Banner;
import com.tapfortap.Interstitial;

abstract class TapForTapBridge
{
    private static final String TAG = "avalon.ad.TapForTapBridge";
    static Activity activity = Cocos2dxHelper.getActivity();
    static Interstitial interstitial = null;
    static RelativeLayout bannerLayout = null;

    public static void init(final String id)
    {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.v(TAG, "init");

                TapForTap.initialize(activity, id);
                interstitial = Interstitial.create(activity);
                interstitial.load();
            }
        });
    }

    public static void showFullscreenAd()
    {
        if (interstitial == null) {
            Log.v(TAG, "showFullscreenAd: NOT INITIALIZED YET!");
            return;
        }

        if (!interstitial.isReadyToShow()) {
            Log.v(TAG, "showFullscreenAd: interstitial not fully loaded yet");
            return;
        }

        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.v(TAG, "showFullscreenAd");

                interstitial.showAndLoad();
            }
        });
    }

    public static void showBanner()
    {
        if (bannerLayout != null) {
            Log.v(TAG, "showBanner: another banner is already visible - ignored");
            return;
        }

        Banner banner = Banner.create(activity);
        banner.enableAutoRollover();

        bannerLayout = new RelativeLayout(activity);
        bannerLayout.setGravity(Gravity.BOTTOM);
        bannerLayout.addView(banner);
        
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.v(TAG, "showBanner");

                FrameLayout mainLayout = (FrameLayout) activity.findViewById(android.R.id.content);
                mainLayout.addView(bannerLayout);
            }
        });
    }

    public static void hideAds()
    {
        if (bannerLayout == null) {
            Log.v(TAG, "hideAds: no banner to hide - ignored");
            return;
        }

        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.v(TAG, "hideAds");

                FrameLayout mainLayout = (FrameLayout) activity.findViewById(android.R.id.content);
                mainLayout.removeView(bannerLayout);

                bannerLayout.removeAllViews();
                bannerLayout = null;
            }
        });
    }
}