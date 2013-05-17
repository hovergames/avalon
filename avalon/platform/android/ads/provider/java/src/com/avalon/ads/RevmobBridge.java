package com.avalon.ads;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.Activity;
//import android.widget.LinearLayout;

import com.revmob.RevMob;
import com.revmob.RevMobTestingMode;
//import com.revmob.ads.banner.RevMobBanner;

abstract class RevmobBridge
{
    private static RevMob revmob;

    public static void init(String id)
    {
        revmob = RevMob.start((Activity) Cocos2dxActivity.getContext(), id);
    }

    public static void showFullscreenAd()
    {
        if (revmob == null) { return; }
        revmob.showFullscreen((Activity) Cocos2dxActivity.getContext());
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
        if (revmob == null) { return; }
        revmob.openAdLink((Activity) Cocos2dxActivity.getContext(), null);
    }

    public static void showPopupAd()
    {
        if (revmob == null) { return; }
        revmob.showPopup((Activity)Cocos2dxActivity.getContext());
    }

    public static void enableTestingWithAds()
    {
        if (revmob == null) { return; }
        revmob.setTestingMode(RevMobTestingMode.WITH_ADS);
    }

    public static void enableTestingWithoutAds()
    {
        if (revmob == null) { return; }
        revmob.setTestingMode(RevMobTestingMode.WITHOUT_ADS);
    }

    public static void disableTesting()
    {
        if (revmob == null) { return; }
        revmob.setTestingMode(RevMobTestingMode.DISABLED);
    }
}
