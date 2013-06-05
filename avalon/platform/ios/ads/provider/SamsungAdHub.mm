#include <avalon/ads/provider/SamsungAdHub.h>

#include <boost/assert.hpp>
#include <avalon/platform/ios/ads/provider/samsungadhub/AdHubInterstitial.h>
#include <avalon/platform/ios/ads/provider/samsungadhub/AdHubInterstitialDelegate.h>
#include <avalon/platform/ios/ads/provider/samsungadhub/AdHubRequestError.h>
#import "AppController.h"
#import "RootViewController.h"

@interface SamsungAdHubDelegate : NSObject<AdHubInterstitialDelegate>
-(void)interstitialAdDidUnload:(AdHubInterstitial*)interstitialAd;
-(void)interstitialAd:(AdHubInterstitial*)interstitialAd didFailWithError:(AdHubRequestError*)error;
-(void)interstitialAdWillLoad:(AdHubInterstitial*)interstitialAd;
@end

@implementation SamsungAdHubDelegate
-(void)interstitialAdDidUnload:(AdHubInterstitial*)interstitialAd
{
    NSLog(@"[SamsungAdHub] interstitialAdDidUnload");
}

-(void)interstitialAd:(AdHubInterstitial*)interstitialAd didFailWithError:(AdHubRequestError*)error
{
    NSLog(@"[SamsungAdHub] didFailWithError: %@", error);
}

-(void)interstitialAdWillLoad:(AdHubInterstitial*)interstitialAd
{
    NSLog(@"[SamsungAdHub] interstitialAdWillLoad");
}
@end

namespace avalon {
namespace ads {
namespace provider {

SamsungAdHub::SamsungAdHub()
: inventoryId()
{
}

void SamsungAdHub::init()
{
    BOOST_ASSERT_MSG(inventoryId != "", "inventoryId must be set first");
}

void SamsungAdHub::hideAds()
{
}

void SamsungAdHub::showFullscreenAd()
{
    NSString* nsId = [NSString stringWithUTF8String:inventoryId.c_str()];
    AdHubInterstitial* interstitial = [[AdHubInterstitial alloc] initWithInventoryID:nsId];
    interstitial.delegate = [SamsungAdHubDelegate alloc];

    AppController* appController = (AppController*) [UIApplication sharedApplication].delegate;
    [interstitial presentFromViewController:appController->viewController];
    [interstitial release];
}

} // namespace provider
} // namespace ads
} // namespace avalon
