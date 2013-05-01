//
//  ChartboostProvider.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 19.04.13.
//
//

#ifndef __Adventures_on_the_farm__ChartboostProvider__
#define __Adventures_on_the_farm__ChartboostProvider__

#include "Interfaces.h"
#include "ChartboostX.h"
#include <string>

namespace Ads
{
    class ChartboostProvider : public ChartboostXDelegate, public Ads::Fullscreen, public Ads::Provider
    {
    public:
        int weight;

        ChartboostProvider();
        
        std::string appId;
        std::string appSignature;
        
        // Ads::Provider
        virtual void init();
        virtual void hideAds();
        virtual int getWeight();

        // Ads::Fullscreen
        virtual void showFullscreenAd();
        
        // ChartboostXDelegate method
        virtual bool shouldDisplayInterstitial(const char* location);
        virtual void didCacheInterstitial(const char* location);
        virtual void didFailToLoadInterstitial(const char* location);
        virtual void didCloseInterstitial(const char* location);
        virtual void didClickInterstitial(const char* location);
    };
}

#endif /* defined(__Adventures_on_the_farm__ChartboostProvider__) */
