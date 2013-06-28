package com.example.tapfortap;

import android.app.Activity;
import android.os.Bundle;

import com.tapfortap.TapForTap;
import com.tapfortap.AdView;
import com.tapfortap.Interstitial;
import com.tapfortap.AppWall;

public class MyActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        // NOTE: Tap for Tap v2 started using an account-wide API key instead of individual app IDs.
        // Please check http://tapfortap.com/account for your API key and use that here.
        TapForTap.initialize(this, "YOUR API KEY");

        setContentView(R.layout.main);
        AdView adView = (AdView) findViewById(R.id.ad_view);

        // To show an interstitial
        Interstitial.prepare(this);
        // then later...
        Interstitial.show(this);

        // To show an App wall
        AppWall.prepare(this);
        // then later...
        AppWall.show(this);
    }

}
