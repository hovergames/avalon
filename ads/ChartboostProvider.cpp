//
//  ChartboostProvider.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 19.04.13.
//
//

#include "ChartboostProvider.h"
#include "ChartboostX.h"
#include "cocos2d.h"

using namespace cocos2d;

namespace Ads
{
    ChartboostProvider::ChartboostProvider() : weight(1)
    {
    }

    void ChartboostProvider::init()
    {
        assert((appId != "") && "appId is empty!");
        assert((appSignature != "") && "appSignature is empty!");

        ChartboostX::sharedChartboostX()->startSession();
        ChartboostX::sharedChartboostX()->setAppId(appId.c_str());
        ChartboostX::sharedChartboostX()->setAppSignature(appSignature.c_str());
        ChartboostX::sharedChartboostX()->setDelegate(this);

        ChartboostX::sharedChartboostX()->cacheInterstitial();
        ChartboostX::sharedChartboostX()->cacheMoreApps();
        CCLog("Chartboost initilized with AppId: %s | AppSig: %s |", appId.c_str(), appSignature.c_str());
    }

    void ChartboostProvider::hideAds()
    {
    }

    int ChartboostProvider::getWeight()
    {
        return weight;
    }

    void ChartboostProvider::showFullscreenAd()
    {
        ChartboostX::sharedChartboostX()->hasCachedInterstitial();
        ChartboostX::sharedChartboostX()->showInterstitial();
    }

    bool ChartboostProvider::shouldDisplayInterstitial(const char* location)
    {
        // we always deliever true because our AdManager will take care of it
        return true;
    }

    void ChartboostProvider::didCacheInterstitial(const char* location)
    {
        CCLog("did cache interstitial at location %s", location);
    }

    void ChartboostProvider::didFailToLoadInterstitial(const char* location)
    {
        CCLog("did fail to load interstitial at location %s", location);
    }

    void ChartboostProvider::didCloseInterstitial(const char* location)
    {
        CCLog("did close interstitial at location %s", location);
    }

    void ChartboostProvider::didClickInterstitial(const char* location)
    {
        CCLog("did click interstitial at location %s", location);
    }

}