package com.avalon;

import android.util.Log;
import android.content.Intent;

import org.cocos2dx.lib.Cocos2dxActivity;
import com.google.example.games.basegameutils.GameHelper;

public abstract class GameCenter
{
    private static GameHelper mHelper = null;
    private static final String TAG = "avalon.GameCenter";
    private static final Cocos2dxActivity activity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
    private final static int REQUEST_ACHIEVEMENTS = 9003;
    private final static int REQUEST_LEADERBOARD = 9004;

    /**
     *
     * Interface used for C++
     *
     */
     
    public static void login()
    {
    }

    public static void signIn()
    {
        activity.runOnUiThread(new Runnable() {
            public void run() {
                getGameHelper().beginUserInitiatedSignIn();
            }
        });
    }

    public static void signOut()
    {
        if (mHelper == null) {
            return;
        }

        activity.runOnUiThread(new Runnable() {
            public void run() {
                getGameHelper().signOut();
            }
        });
    }

    public static boolean isSignedIn()
    {
        return (mHelper != null && getGameHelper().isSignedIn());
    }

    public static boolean showAchievements()
    {
        if (!isSignedIn()) {
            return false;
        }
        
        activity.runOnUiThread(new Runnable() {
            public void run() {
                activity.startActivityForResult(
                    getGameHelper().getGamesClient().getAchievementsIntent(),
                    REQUEST_ACHIEVEMENTS
                );
            }
        });
        return true;
    }

    public static void postAchievement(final String idName, int percentComplete)
    {
        if (!isSignedIn() || percentComplete < 100) {
            return;
        }

        activity.runOnUiThread(new Runnable() {
            public void run() {
                int rId = getResourceId(idName);
                if (rId > 0) {
                    String googleId = activity.getResources().getString(rId);
                    getGameHelper().getGamesClient().unlockAchievement(googleId);
                }
            }
        });
    }

    public static void clearAllAchievements()
    {
        Log.v(TAG, "clearAllAchievements is not available on this platform");
    }

    public static boolean showScores()
    {
        if (!isSignedIn()) {
            return false;
        }
        
        activity.runOnUiThread(new Runnable() {
            public void run() {
                activity.startActivityForResult(
                    getGameHelper().getGamesClient().getAllLeaderboardsIntent(),
                    REQUEST_LEADERBOARD
                );
            }
        });
        return true;
    }

    public static void postScore(final String idName, final int score)
    {
        if (!isSignedIn()) {
            return;
        }

        activity.runOnUiThread(new Runnable() {
            public void run() {
                int rId = getResourceId(idName);
                if (rId > 0) {
                    String googleId = activity.getResources().getString(rId);
                    getGameHelper().getGamesClient().submitScore(googleId, score);
                }
            }
        });
    }

    public static void clearAllScores()
    {
        Log.v(TAG, "clearAllScores is not available on this platform");
    }

    /**
     *
     * Public interface to integrate this into the main activity
     *
     */
     
    public static void onStart()
    {
        getGameHelper().onStart(activity);
    }

    public static void onStop()
    {
        if (mHelper == null) {
            return;
        }

        getGameHelper().onStop();
    }

    public static void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        if (mHelper == null) {
            return;
        }

        getGameHelper().onActivityResult(requestCode, resultCode, data);
    }

    /**
     *
     * Private helper methods
     *
     */

    private static int getResourceId(String idName)
    {
        int rId = activity.getResources().getIdentifier(idName, "string", activity.getPackageName());
        if (rId == 0) {
            Log.v(TAG, "Unable to find resource ID for string " + idName);
        }
        return rId;

    }

    private static GameHelper getGameHelper()
    {
        if (mHelper == null) {
            mHelper = new GameHelper(activity);
            mHelper.enableDebugLog(true, TAG);

            activity.runOnUiThread(new Runnable() {
                public void run() {
                    mHelper.setup(
                        new GameHelper.GameHelperListener() {
                            @Override
                            public void onSignInFailed() {
                                Log.v(TAG, "onSignInFailed");
                            }

                            @Override
                            public void onSignInSucceeded() {
                                Log.v(TAG, "onSignInSucceeded");
                            }
                        }, GameHelper.CLIENT_GAMES
                    );
                }
            });
        }

        return mHelper;
    }
}