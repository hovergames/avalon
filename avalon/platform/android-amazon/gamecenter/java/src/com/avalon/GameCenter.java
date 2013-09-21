package com.avalon;

import java.util.EnumSet;

import android.app.Activity;
import android.util.Log;

import com.amazon.ags.api.AmazonGamesCallback;
import com.amazon.ags.api.AmazonGamesClient;
import com.amazon.ags.api.AmazonGamesFeature;
import com.amazon.ags.api.AmazonGamesStatus;
import com.amazon.ags.api.AGResponseCallback;
import com.amazon.ags.api.AGResponseHandle;
import com.amazon.ags.api.RequestResponse;
import com.amazon.ags.api.leaderboards.LeaderboardsClient;
import com.amazon.ags.api.leaderboards.SubmitScoreResponse;
import com.amazon.ags.api.achievements.AchievementsClient;
import com.amazon.ags.api.achievements.UpdateProgressResponse;

import org.cocos2dx.lib.Cocos2dxHelper;

public abstract class GameCenter
{
    private static AmazonGamesClient agsGameClient = null;
    private static LeaderboardsClient lbClient = null;
    private static AchievementsClient acClient = null;
    private static final String TAG = "avalon.GameCenter";

    public static void login()
    {
        if (agsGameClient != null) {
            return;
        }

        Cocos2dxHelper.getActivity().runOnUiThread(new Runnable() {
            public void run() {
                 AmazonGamesClient.initialize(
                    Cocos2dxHelper.getActivity(),
                    agsGameCallback,
                    EnumSet.of(AmazonGamesFeature.Achievements, AmazonGamesFeature.Leaderboards)
                );
            }
        });
    }

    public static boolean showAchievements()
    {
        if (agsGameClient == null || !agsGameClient.isInitialized() || acClient == null) {
            return false;
        } else {
            acClient.showAchievementsOverlay();
            return true;
        }
    }

    public static void postAchievement(String idName, int percentComplete)
    {
        if (agsGameClient == null || !agsGameClient.isInitialized() || acClient == null) {
            return;
        }

        AGResponseHandle<UpdateProgressResponse> handle = acClient.updateProgress(idName, percentComplete);
        handle.setCallback(new AGResponseCallback<UpdateProgressResponse>() {
            @Override
            public void onComplete(UpdateProgressResponse result) {
                if (result.isError()) {
                    Log.v(TAG, "postAchievement failed: " + result.getError());
                }
            }
        });
    }

    public static void clearAllAchievements()
    {
    }

    public static boolean showScores()
    {
        if (agsGameClient == null || !agsGameClient.isInitialized() || lbClient == null) {
            return false;
        } else {
            lbClient.showLeaderboardsOverlay();
            return true;
        }
    }

    public static void postScore(String idName, int score)
    {
        if (agsGameClient == null || !agsGameClient.isInitialized() || lbClient == null) {
            return;
        }

        AGResponseHandle<SubmitScoreResponse> handle = lbClient.submitScore(idName, score);
        handle.setCallback(new AGResponseCallback<SubmitScoreResponse>() {
            @Override
            public void onComplete(SubmitScoreResponse result) {
                if (!result.isError()) {
                    Log.v(TAG, "postScore failed: " + result.getError());
                }
            }
        });
    }

    public static void clearAllScores()
    {
    }

    private static final AmazonGamesCallback agsGameCallback = new AmazonGamesCallback()
    {
        @Override
        public void onServiceReady(AmazonGamesClient amazonGamesClient) {
            agsGameClient = amazonGamesClient;
            lbClient = agsGameClient.getLeaderboardsClient();
            acClient = agsGameClient.getAchievementsClient();
        }

        @Override
        public void onServiceNotReady(AmazonGamesStatus reason) {
            // result is ignored
        }
    };
}
