#include <avalon/ads/provider/Chartboost.h>

#include <boost/assert.hpp>
#include <avalon/platform/ios/ads/provider/Chartboost/Chartboost.h>

namespace avalon {
namespace ads {
namespace provider {

Chartboost::Chartboost()
: appId()
, appSignature()
{
}

void Chartboost::init()
{
    BOOST_ASSERT_MSG(!appId.empty(), "appId must be set first");
    BOOST_ASSERT_MSG(!appSignature.empty(), "appSignature must be set first");

    auto cb = [::Chartboost sharedChartboost];
    cb.appId = [NSString stringWithUTF8String:appId.c_str()];
    cb.appSignature = [NSString stringWithUTF8String:appSignature.c_str()];

    [cb startSession];
    [cb cacheInterstitial];
    [cb cacheMoreApps];
}

void Chartboost::hideAds()
{
}

void Chartboost::showFullscreenAd()
{
    auto cb = [::Chartboost sharedChartboost];
    [cb showInterstitial];
    [cb cacheInterstitial];
}

void Chartboost::openAdLink()
{
    auto cb = [::Chartboost sharedChartboost];
    [cb showMoreApps];
    [cb cacheMoreApps];
}

} // namespace provider
} // namespace ads
} // namespace avalon
