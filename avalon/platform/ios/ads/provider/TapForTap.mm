#ifdef AVALON_CONFIG_ADS_PROVIDER_TAPFORTAP_ENABLED

#include <avalon/ads/provider/TapForTap.h>

#import <CoreLocation/CoreLocation.h>
#include <boost/assert.hpp>
#import <avalon/platform/ios/ads/provider/TapForTap/TapForTap.h>

#import "AppController.h"
#import "RootViewController.h"

namespace avalon {
namespace ads {
namespace provider {

TapForTap::TapForTap()
: apiKey()
{
}

void TapForTap::init()
{
    BOOST_ASSERT_MSG(!apiKey.empty(), "apiKey must be set first");
    [::TapForTap initializeWithAPIKey:[NSString stringWithUTF8String:apiKey.c_str()]];

#ifdef AVALON_PLATFORM_IOS_USE_CORELOCATION
    CLLocationManager* locationManager = [[CLLocationManager alloc] init];
    [locationManager startUpdatingLocation];
    [::TapForTap setLocation:locationManager.location];
#else
    NSLog(@"[TapForTap] Increase your eCPM with AVALON_PLATFORM_IOS_USE_CORELOCATION!");
#endif
}

void TapForTap::hideAds()
{
    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;
    for (UIView* subView in appController->viewController.view.subviews) {
        if (static_cast<TapForTapAdView*>(subView) != NULL) {
            [subView removeFromSuperview];
        }
    }
    [TapForTapInterstitial close];
}

void TapForTap::showFullscreenAd()
{
    [TapForTapInterstitial prepare];

    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;
    [TapForTapInterstitial showWithRootViewController:appController->viewController];
}

void TapForTap::showBanner()
{
    // TapForTap stores banner ads in 320x50 and 640x100 (retina) and the SDK
    // selects the ad that fits in the size we give to initWithFrame. So we
    // simply detect if we're wide enough for the 100px width banner.
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat adHeight = (screenRect.size.width < 640) ? 50 : 100;
    CGFloat posY = screenRect.size.height - adHeight;
    
    TapForTapAdView* adView = [[TapForTapAdView alloc] initWithFrame:CGRectMake(0, posY, screenRect.size.width, adHeight)];
    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;
    [appController->viewController.view addSubview:adView];
    [adView release];
}

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_CONFIG_ADS_PROVIDER_TAPFORTAP_ENABLED */
