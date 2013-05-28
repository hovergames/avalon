package com.avalon;

import java.util.EnumSet;

import android.app.Activity;
import android.util.Log;

import com.amazon.ags.api.AmazonGames;
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

import org.cocos2dx.lib.Cocos2dxActivity;

public abstract class GameCenter
{
    private static AmazonGames agsGameClient;
    private static LeaderboardsClient lbClient;
    private static AchievementsClient acClient;
    private static final String TAG = "avalon.GameCenter";

    public static void login()
    {
        if (agsGameClient != null) {
            return;
        }
        
        final Cocos2dxActivity activity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
        activity.runOnUiThread(new Runnable() {
            public void run() {
                GameCenter.agsGameClient = AmazonGamesClient.initialize(
                    activity.getApplication(),
                    agsGameCallback,
                    EnumSet.of(AmazonGamesFeature.Achievements, AmazonGamesFeature.Leaderboards)
                );
            }
        });
    }

    public static boolean showAchievements()
    {
        if (agsGameClient == null || !agsGameClient.isReady()) {
            return false;
        }

        acClient.showAchievementsOverlay();
        return true;
    }

    public static void postAchievement(String idName, int percentComplete)
    {
        if (agsGameClient == null || !agsGameClient.isReady()) {
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
        if (agsGameClient == null || !agsGameClient.isReady()) {
            return;
        }
        
        AGResponseHandle<RequestResponse> handle = acClient.resetAchievements();
        handle.setCallback(new AGResponseCallback<RequestResponse>() {
            @Override
            public void onComplete(RequestResponse result) {
                if (result.isError()) {
                    Log.v(TAG, "clearAllAchievements failed: " + result.getError());
                }
            }
        });
    }

    public static boolean showScores()
    {
        if (agsGameClient == null || !agsGameClient.isReady()) {
            return false;
        }

        lbClient.showLeaderboardsOverlay();
        return true;
    }

    public static void postScore(String idName, int score)
    {
        if (agsGameClient == null || !agsGameClient.isReady()) {
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
        public void onServiceReady() {
            lbClient = agsGameClient.getLeaderboardsClient();
            acClient = agsGameClient.getAchievementsClient();
        }

        @Override
        public void onServiceNotReady(AmazonGamesStatus reason) {
            // result is ignored
        }
    };

}