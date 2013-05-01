//
//  AdManager.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 19.04.13.
//
//

#ifndef __Adventures_on_the_farm__AdManager__
#define __Adventures_on_the_farm__AdManager__

#include "Interfaces.h"
#include <vector>
#include "cocos2d.h"
#include <time.h>

namespace Ads
{
    class AdManager
    {
    public:
        static std::vector<Provider*> adProviders;

        static time_t cooldown;
        static int onlyShowEveryNThAd;
        static int dontShowFirstAdOnAppStart;

        static int adCount;
        static time_t lastAdShownAt;
        static bool enabled;

        AdManager() {}
        ~AdManager() {}

        static AdManager *adManager;

        static void initWithIniFile(const char *iniFile);
        static void startService();

        static void showFullscreenAd();
        static void showBanner();
        static void showPopup();
        static void openAdLink();

        static bool showNextAd();
        static void hide();
        
        static void enableAds();
        static void disableAds();
        
        template <typename T> static T* getRandomProvider(std::vector<Ads::Provider*>)
        {
            float sumWeight = 0.0;
            for (std::vector<Provider*>::iterator it = adProviders.begin(); it != adProviders.end(); ++it)
            {
                T *p = dynamic_cast<T*>(*it);
                if (p)
                {
                    sumWeight += (*it)->getWeight();
                }
            }

            T *selectedProvider;
            
            float randomNumber =  static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / sumWeight);
            for (std::vector<Provider*>::iterator it = adProviders.begin(); it != adProviders.end(); ++it)
            {
                T *p = dynamic_cast<T*>(*it);
                if (p)
                {
                    randomNumber -= (*it)->getWeight();
                    if (randomNumber <= 0)
                    {
                        selectedProvider = p;
                        break;
                    }
                }
            }

            return selectedProvider;
        }
    };

}


#endif /* defined(__Adventures_on_the_farm__AdManager__) */
