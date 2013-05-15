#include <avalon/platform/ios/ads/provider/RevmobBridge.h>

#import <RevMobAds/RevMobAds.h>

namespace avalon {
namespace ads {
namespace provider {

void RevmobBridge::startSession(const char* sessionId)
{
    NSString* nsId = [NSString stringWithUTF8String:sessionId];
    [RevMobAds startSessionWithAppID:nsId];
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
