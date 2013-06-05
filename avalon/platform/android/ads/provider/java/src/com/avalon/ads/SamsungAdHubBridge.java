package com.avalon.ads;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.Activity;
import android.util.Log;

import com.sec.android.ad.AdHubInterstitial;
import com.sec.android.ad.AdInterstitialListener;

abstract class SamsungAdHubBridge
{
    static final String TAG = "avalon.ads.SamsungAdHubBridge";
    public static AdHubInterstitial adHubInterstitial;

    public static void init(final String id)
    {
        final Activity activity = (Activity) Cocos2dxActivity.getContext();
        activity.runOnUiThread(new Runnable() {
            public void run() {
                SamsungAdHubBridge.adHubInterstitial = new AdHubInterstitial(Cocos2dxActivity.getContext(), id);
                SamsungAdHubBridge.adHubInterstitial.setListener(new AdInterstitialListener() {
                    @Override
                    public void onAdInterstitialReceived(AdHubInterstitial ad) {
                        Log.v(TAG, "onAdInterstitialReceived");
                    }

                    @Override
                    public void onAdInterstitialFailed(AdHubInterstitial ad, Exception e) {
                        Log.v(TAG, "onAdInterstitialFailed: " + e.getMessage());
                    }
                    
                    @Override
                    public void onAdInterstitialClosed(AdHubInterstitial ad) {
                        Log.v(TAG, "onAdInterstitialClosed");
                    }
                });
            }
        });
    }

    public static void showFullscreenAd()
    {
        if (adHubInterstitial == null) { return; }
        adHubInterstitial.startAd();
    }

    public static void hideAds()
    {
    }
}