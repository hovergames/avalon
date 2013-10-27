package com.avalon.ads;

import android.util.Log;
import android.app.Activity;

import org.cocos2dx.lib.Cocos2dxHelper;
import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.ChartboostDelegate;

abstract class ChartboostBridge
{
    static final String TAG = "avalon.ads.ChartboostBridge";
    private static Activity activity = Cocos2dxHelper.getActivity();
    private static Chartboost cb = null;

    public static void init(final String appId, final String appSignature)
    {
        if (cb != null) {
            Log.v(TAG, "init: already initialized - ignored");
            return;
        }

        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.v(TAG, "init");

                cb = Chartboost.sharedChartboost();
                cb.setImpressionsUseActivities(true);

                cb.onCreate(activity, appId, appSignature, getDelegate());
                cb.startSession();
                cb.onStart(activity);

                // prefill all the caches!
                cb.cacheInterstitial();
                cb.cacheMoreApps();
            }
        });
    }

    public static void showFullscreenAd()
    {
        if (cb == null) {
            Log.v(TAG, "showFullscreenAd: NOT INITIALIZED YET!");
            return;
        }

        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.v(TAG, "showFullscreenAd");

                cb.showInterstitial();
                cb.cacheInterstitial();
            }
        });
    }

    public static void hideAds()
    {
        Log.v(TAG, "hideAds");
    }

    public static void openAdLink()
    {
        if (cb == null) {
            Log.v(TAG, "openAdLink: NOT INITIALIZED YET!");
            return;
        }

        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.v(TAG, "openAdLink");

                cb.showMoreApps();
                cb.cacheMoreApps();
            }
        });
    }

    public static ChartboostDelegate getDelegate()
    {
        return new ChartboostDelegate()
        {
            public void didCacheInterstitial(String location)
            {
                Log.v(TAG, "didCacheInterstitial: " + location);
            }

            public void didFailToLoadInterstitial(String location)
            {
                Log.v(TAG, "didFailToLoadInterstitial: " + location);
            }

            public void didDismissInterstitial(String location)
            {
                Log.v(TAG, "didDismissInterstitial: " + location);
            }

            public void didCloseInterstitial(String location)
            {
                Log.v(TAG, "didCloseInterstitial: " + location);
            }

            public void didClickInterstitial(String location)
            {
                Log.v(TAG, "didClickInterstitial: " + location);
            }

            public void didShowInterstitial(String location)
            {
                Log.v(TAG, "didShowInterstitial: " + location);
            }

            public boolean shouldDisplayInterstitial(String location)
            {
                return true;
            }

            public boolean shouldDisplayLoadingViewForMoreApps()
            {
                return true;
            }

            public boolean shouldRequestMoreApps()
            {
                return true;
            }

            public void didCacheMoreApps()
            {
                Log.v(TAG, "didCacheMoreApps");
            }

            public boolean shouldDisplayMoreApps()
            {
                return true;
            }

            public void didFailToLoadMoreApps()
            {
                Log.v(TAG, "didFailToLoadMoreApps");
            }

            public void didDismissMoreApps()
            {
                Log.v(TAG, "didDismissMoreApps");
            }

            public void didCloseMoreApps()
            {
                Log.v(TAG, "didCloseMoreApps");
            }

            public void didClickMoreApps()
            {
                Log.v(TAG, "didClickMoreApps");
            }

            public void didShowMoreApps()
            {
                Log.v(TAG, "didShowMoreApps");
            }

            public boolean shouldRequestInterstitial(String location)
            {
                return true;
            }

            public boolean shouldRequestInterstitialsInFirstSession()
            {
                return true;
            }
        };
    }
}
