#include <avalon/ads/provider/Chartboost.h>

#include <boost/assert.hpp>
#include "cocos2d.h"

using namespace cocos2d;

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
    BOOST_ASSERT_MSG(!appId.empty(), "appId is empty!");
    BOOST_ASSERT_MSG(!appSignature.empty(), "appSignature is empty!");

    ChartboostX::sharedChartboostX()->setAppId(appId.c_str());
    ChartboostX::sharedChartboostX()->setAppSignature(appSignature.c_str());
    ChartboostX::sharedChartboostX()->setDelegate(this);
    ChartboostX::sharedChartboostX()->startSession();

    ChartboostX::sharedChartboostX()->cacheInterstitial();
    ChartboostX::sharedChartboostX()->cacheMoreApps();
    CCLog("[Chartboost] init AppId: %s and AppSig: %s", appId.c_str(), appSignature.c_str());
}

void Chartboost::hideAds()
{
}

void Chartboost::showFullscreenAd()
{
    ChartboostX::sharedChartboostX()->hasCachedInterstitial();
    ChartboostX::sharedChartboostX()->showInterstitial();
}

void Chartboost::openAdLink()
{
    ChartboostX::sharedChartboostX()->showMoreApps();
    ChartboostX::sharedChartboostX()->cacheMoreApps();
}

bool Chartboost::shouldDisplayInterstitial(const char* location)
{
    // we always deliever true because our AdManager will take care of it
    return true;
}

void Chartboost::didCacheInterstitial(const char* location)
{
    CCLog("[Chartboost] didCacheInterstitial %s", location);
}

void Chartboost::didFailToLoadInterstitial(const char* location)
{
    CCLog("[Chartboost] didFailToLoadInterstitial %s", location);
}

void Chartboost::didCloseInterstitial(const char* location)
{
    CCLog("[Chartboost] didCloseInterstitial %s", location);
}

void Chartboost::didClickInterstitial(const char* location)
{
    CCLog("[Chartboost] didClickInterstitial %s", location);
}

} // namespace provider
} // namespace ads
} // namespace avalon