package com.avalon.ads;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.Activity;
//import android.widget.LinearLayout;

import com.revmob.RevMob;
import com.revmob.RevMobTestingMode;
//import com.revmob.ads.banner.RevMobBanner;

abstract class RevmobBridge
{
    static Cocos2dxActivity activity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
    static RevMob revmob = null;

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
        /**
         * TODO: Figure out how to get the layout thing right with cocos2dx
         *
        if (revmob == null) { return; }

        final RevMobBanner banner = revmob.createBanner((Activity)Cocos2dxActivity.getContext());
        final Activity activity = (Activity) Cocos2dxActivity.getContext();
        
        activity.runOnUiThread(new Runnable() {
            public void run() {
                LinearLayout layout = (LinearLayout) activity.findViewById(R.id.banner);
                layout.removeAllViews();
                layout.addView(banner);
            }
        });
        */
    }

    public static void hideAds()
    {
        /**
         * TODO: Figure out how to get the layout thing right with cocos2dx
         *
        final Activity activity = (Activity) Cocos2dxActivity.getContext();
        activity.runOnUiThread(new Runnable() {
            public void run() {
                LinearLayout layout = (LinearLayout) activity.findViewById(R.id.banner);
                layout.removeAllViews();
            }
        });
        */
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
