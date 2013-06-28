#include <avalon/platform/ios/ads/provider/RevmobBridge.h>

#import <CoreLocation/CoreLocation.h>
#import <RevMobAds/RevMobAds.h>

namespace avalon {
namespace ads {
namespace provider {

void RevmobBridge::startSession(const char* sessionId)
{
    NSString* nsId = [NSString stringWithUTF8String:sessionId];
    [RevMobAds startSessionWithAppID:nsId];

#ifdef AVALON_PLATFORM_IOS_USE_CORELOCATION
    CLLocationManager* locationManager = [[CLLocationManager alloc] init];
    [locationManager startUpdatingLocation];

    CLLocation* location = locationManager.location;
    [[RevMobAds session] setUserLatitude:location.coordinate.latitude
                          userLongitude:location.coordinate.longitude
                 userHorizontalAccuracy:location.horizontalAccuracy
                           userAltitude:location.altitude
                   userVerticalAccuracy:location.verticalAccuracy];
#else
    NSLog(@"[Revmob] Increase your eCPM with AVALON_PLATFORM_IOS_USE_CORELOCATION!");
#endif
}

void RevmobBridge::showFullscreen()
{
    [[RevMobAds session] showFullscreen];
}

void RevmobBridge::showBanner()
{
    [[RevMobAds session] showBanner];
}

void RevmobBridge::hideBanner()
{
    [[RevMobAds session] hideBanner];
}

void RevmobBridge::openAdLink()
{
    [[RevMobAds session] openAdLinkWithDelegate:nil];
}

void RevmobBridge::showPopup()
{
    [[RevMobAds session] showPopup];
}

void RevmobBridge::enableTestingWithAds()
{
    [RevMobAds session].testingMode = RevMobAdsTestingModeWithAds;
}

void RevmobBridge::enableTestingWithoutAds()
{
    [RevMobAds session].testingMode = RevMobAdsTestingModeWithoutAds;
}

void RevmobBridge::disableTesting()
{
    [RevMobAds session].testingMode = RevMobAdsTestingModeOff;
}

} // namespace provider
} // namespace ads
} // namespace avalon
