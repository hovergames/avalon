
2.3.0 / 2013-04-03 

  * Earning can now be paused from the web interface
  * Added new events to app wall and interstitial
    * onShow
    * onReceiveAd
  * AdView's can now be buffered by calling setBuffer(true). This allows you to hide the adView until an ad has been downloaded.
  * Upgraded AdMob to 6.3.1
  * Added support for AdMob interstitial mediation

### 2.2.0 / 2013-03-11 

  * Cache AppWall/Interstitial ad. Results is smoother displaying of full screen ads.
  * Add onFail to interstital and app-wall

### 2.1.2 / 2013-02-05 

  * Fixed determining width of Ad banner.
  * Fixed a rare occurence of a bad URL. 

### 2.1.1 / 2013-01-14 

  * Respect gravity setting on AdView
  * Remove dependency on Activity in TapForTap.initialize
  * Removed AppWall and Interstitial dependency on Activity (uses ApplicationContext)
  * Remove AdView dependency on Activity (uses ApplicationContext)
  * Fix a crash when going to the app wall from the pitch page
  * Fix a potential crash in AppWall and Interstitial if the API is used in an undocumented way
  * Fix an issue where interstitials and app walls may show repeatedly
  * Various minor bug fixes

### 2.1.0 / 2012-11-15 

  * Include TapForTap.jar in TapForTapAdMob.jar
  * Add interstitial & app wall listeners with one method: void onDismiss()
  * Accept more user info: year of birth, user account IDs for the app
  * Start requests on the UI thread for PhoneGap compatibility
  * Improve feedback about integration problems (API key, etc.)
  * Build against Android 4.2 SDK
  * Improve usefulness of example project
  * Fix a null pointer exception when not initialized
  * Fix a concurrency exception, triggered in rare cases
  * Paranoid fix for a rare IndexOutOfBounds exception
  * Force initialization if the API key changes
  * Handle a weird edge case where the AssetManager is null
  * Handle a weird edge case where AdView width/height are zero
  * AdView constructor accepts real Contexts now

### 2.0.2 / 2012-10-24 

  * Remove the check for enough space to display banners
  * Add support for AdMob 6.2
  * Add interstitial and app wall to example code
  * Fix a bug where ads take a long time to load

### 2.0.1 / 2012-10-16 

  * Fix two NullPointerException bugs
  * Fix a potential crashing bug when the WRITE\_EXTERNAL\_STORAGE permission is missing
  * Update example code

### 2.0.0 / 2012-10-15 

  * Major new version!
  * Add interstitials
  * App an app wall
  * Banners are now full-width
  * Add apps simply by integrating the SDK

### 1.1.14 / 2012-09-17 

  * Fix two bugs on Android 3+
  * Fix backwards compatibility (was broken pre-Gingerbread, API level 9)

### 1.1.13 / 2012-09-14

  * Optimize data transferred over the network
  * Automatically enable test mode for debug builds

### 1.1.12 / 2012-08-31

  * Fix a scaling issue when the ad view is height constrained
  * Add TapForTap.testMode flag

### 1.1.11 / 2012-08-10

  * Improve documentation (put Eclipse/IDEA instructions behind tabs)
  * Mention creating an AdView without using XML layout

### 1.1.10 / 2012-07-30

  * Break out AdMob into a separate JAR for distribution
  * Fix a potential crashing bug when an image is fetched after the AdView is released
  * Fix sizing issues with AdMob
  * AdMobBanner uses the server param as an app ID if present

### 1.1.9 / 2012-07-19

  * Show a message when trying to open market URLs in the emulator
  * fix crashing bug when tapping ads but no ads loaded
  * fix a bug where tapping on ads sometimes does nothing

### 1.1.8 / 2012-07-09

  * Add com.tapfortap.AdMobBanner adapter for AdMob mediation
  * Add AdView.autoRollover for AdMob
  * Scale ads intelligently to fit space
  * Improve detection for tablets
  * Call AdViewListener methods on the UI thread

### 1.1.7 / 2012-06-05

  * Make READ_PHONE_STATE permission optional
  * We do not need the ACCESS_WIFI_STATE permission
  * Mention Android permissions for the manifest in the readme

### 1.1.6 / 2012-05-17

  * Fix a bug when trying to get network info when offline
    (Critical fix for all versions of Android, potential crasher)


### 1.1.5 / 2012-05-16

  * Fix a bug where tapping an ad may not load the URL on a background thread.
    (Critical fix for Android 3.2+, crasher)
  * send network type when filling ads (wifi or cellular)


### 1.1.4 / 2012-05-07

  * only check in once
  * when filling ads encode params in UTF-8


### 1.1.3 / 2012-04-23

  * Fix a bug where images may not be downloaded on a background thread.
    (Critical fix for Android 3.2+, crasher)


### 1.1.2 / 2012-04-18

  * Add AdViewListener for notifications about receiving or failing to receive ads.


### 1.1.1 / 2012-04-13

  * Fix a crash on PhoneGap 1.6.0
  * Fix some potential null pointer exceptions
  * add a changelog
