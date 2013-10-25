#ifdef AVALON_CONFIG_ADS_PROVIDER_REVMOB_ENABLED

#include <avalon/ads/provider/Revmob.h>

#include <boost/assert.hpp>
#import <CoreLocation/CoreLocation.h>
#import <RevMobAds/RevMobAds.h>

static RevMobFullscreen* fullscreen = nullptr;

namespace avalon {
namespace ads {
namespace provider {

Revmob::Revmob()
: appId()
{
}

void Revmob::init()
{
    BOOST_ASSERT_MSG(!appId.empty(), "appId must be set first");
    [RevMobAds startSessionWithAppID:[NSString stringWithUTF8String:appId.c_str()]];

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

    fullscreen = [[[RevMobAds session] fullscreen] retain];
    [fullscreen loadAd];
}

void Revmob::hideAds()
{
    [[RevMobAds session] hideBanner];
}

void Revmob::showFullscreenAd()
{
    if (fullscreen) {
        [fullscreen showAd];
        [fullscreen release];
        fullscreen = nullptr;
    }

    fullscreen = [[[RevMobAds session] fullscreen] retain];
    [fullscreen loadAd];
}

void Revmob::openAdLink()
{
    [[RevMobAds session] openAdLinkWithDelegate:NULL];
}

void Revmob::showBanner()
{
    [[RevMobAds session] showBanner];
}

void Revmob::showPopupAd()
{
    [[RevMobAds session] showPopup];
}

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_CONFIG_ADS_PROVIDER_REVMOB_ENABLED */