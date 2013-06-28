package com.avalon.ads;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.Activity;
import android.widget.RelativeLayout;
import android.widget.FrameLayout;
import android.view.Gravity;

import com.revmob.RevMob;
import com.revmob.RevMobTestingMode;

abstract class RevmobBridge
{
    static Cocos2dxActivity activity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
    static RevMob revmob = null;
    static RelativeLayout adView = null;

    public static void init(String id)
    {
        if (revmob == null) {
            revmob = RevMob.start(activity, id);
        }
    }

    public static void showFullscreenAd()
    {
        if (revmob != null) {
            revmob.showFullscreen(activity);
        }
    }

    public static void showBanner()
    {
        if (revmob == null || adView != null) {
            return;
        }

        adView = new RelativeLayout(activity);
        adView.setGravity(Gravity.BOTTOM);
        adView.addView(revmob.createBanner(activity));

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

    public static void openAdLink()
    {
        if (revmob != null) {
            revmob.openAdLink(activity, null);
        }
    }

    public static void showPopupAd()
    {
        if (revmob != null) {
            revmob.showPopup(activity);
        }
    }

    public static void enableTestingWithAds()
    {
        if (revmob != null) {
            revmob.setTestingMode(RevMobTestingMode.WITH_ADS);
        }
    }

    public static void enableTestingWithoutAds()
    {
        if (revmob != null) {
            revmob.setTestingMode(RevMobTestingMode.WITHOUT_ADS);
        }
    }

    public static void disableTesting()
    {
        if (revmob != null) {
            revmob.setTestingMode(RevMobTestingMode.DISABLED);
        }
    }
}
