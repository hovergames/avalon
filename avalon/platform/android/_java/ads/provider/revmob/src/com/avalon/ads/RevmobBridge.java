package com.avalon.ads;

import org.cocos2dx.lib.Cocos2dxHelper;

import android.app.Activity;
import android.util.Log;
import android.widget.RelativeLayout;
import android.widget.FrameLayout;
import android.view.Gravity;

import com.revmob.RevMob;
import com.revmob.RevMobAdsListener;
import com.revmob.ads.fullscreen.RevMobFullscreen;
import com.revmob.ads.link.RevMobLink;
import com.revmob.ads.banner.RevMobBanner;

abstract class RevmobBridge
{
    static final String TAG = "avalon.ads.RevmobBridge";
    private static Activity activity = Cocos2dxHelper.getActivity();
    private static RevMob revmob = null;
    private static RevMobFullscreen fullscreen = null;
    private static RevMobLink adLink = null;
    private static RevMobBanner banner = null;
    private static RelativeLayout adView = null;

    public static void init(final String id)
    {
        if (revmob != null) {
            Log.v(TAG, "init: already initialized - ignored");
            return;
        }

        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.v(TAG, "init");

                revmob = RevMob.start(activity, id);
                
                // prefill all the caches!
                fullscreen = revmob.createFullscreen(activity, getListener("FullscreenAd"));
                banner = revmob.createBanner(activity, getListener("Banner"));
                adLink = revmob.createAdLink(activity, getListener("AdLink"));
            }
        });
    }

    public static void showFullscreenAd()
    {
        if (revmob == null) {
            Log.v(TAG, "showFullscreenAd: NOT INITIALIZED YET!");
            return;
        }

        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.v(TAG, "showFullscreenAd");

                if (fullscreen != null) {
                    fullscreen.show();
                }

                // pre-load next fullscreen ad
                fullscreen = revmob.createFullscreen(activity, getListener("FullscreenAd"));
            }
        });
    }

    public static void showBanner()
    {
        if (revmob == null) {
            Log.v(TAG, "showBanner: NOT INITIALIZED YET!");
            return;
        }

        if (adView != null) {
            Log.v(TAG, "showBanner: another banner should already be visible - ignored");
            return;
        }

        activity.runOnUiThread(new Runnable() {
            public void run() {
                Log.v(TAG, "showBanner");

                adView = new RelativeLayout(activity);
                adView.setGravity(Gravity.BOTTOM);
                adView.addView(banner);

                FrameLayout mainLayout = (FrameLayout) activity.findViewById(android.R.id.content);
                mainLayout.addView(adView);

                // pre-load next banner
                banner = revmob.createBanner(activity, getListener("Banner"));
            }
        });
    }

    public static void hideAds()
    {
        activity.runOnUiThread(new Runnable() {
            public void run() {
                Log.v(TAG, "hideAds");

                if (fullscreen != null) {
                    fullscreen.hide();
                }

                if (adView != null) {
                    FrameLayout mainLayout = (FrameLayout) activity.findViewById(android.R.id.content);
                    mainLayout.removeView(adView);

                    adView.removeAllViews();
                    adView = null;
                }
            }
        });
    }

    public static void openAdLink()
    {
        if (revmob == null) {
            Log.v(TAG, "openAdLink: NOT INITIALIZED YET!");
            return;
        }

        activity.runOnUiThread(new Runnable() {
            public void run() {
                Log.v(TAG, "openAdLink");

                if (adLink != null) {
                    adLink.open();
                }

                // pre-load next banner
                adLink = revmob.createAdLink(activity, getListener("AdLink"));
            }
        });
    }

    public static void showPopupAd()
    {
        if (revmob == null) {
            Log.v(TAG, "showPopupAd: NOT INITIALIZED YET!");
            return;
        }

        activity.runOnUiThread(new Runnable() {
            public void run() {
                Log.v(TAG, "showPopupAd");

                revmob.showPopup(activity, null, getListener("PopUpAd"));
            }
        });
    }

    public static RevMobAdsListener getListener(final String mode)
    {
        return new RevMobAdsListener()
        {
            public void onRevMobAdReceived()
            {
                Log.v(TAG, mode + ": onRevMobAdReceived");
            }

            public void onRevMobAdNotReceived(String message)
            {
                Log.v(TAG, mode + ": onRevMobAdNotReceived: " + message);
            }

            public void onRevMobAdDisplayed()
            {
                Log.v(TAG, mode + ": onRevMobAdDisplayed");
            }

            public void onRevMobAdDismiss()
            {
                Log.v(TAG, mode + ": onRevMobAdDismiss");
            }

            public void onRevMobAdClicked()
            {
                Log.v(TAG, mode + ": onRevMobAdClicked");
            }
        };
    }
}
