#ifndef AVALON_ADS_MANAGER_H
#define AVALON_ADS_MANAGER_H

#include <vector>
#include <time.h>
#include "cocos2d.h"
#include <avalon/ads/Provider.h>

namespace avalon {
namespace ads {

class Manager
{
public:
    static std::vector<Provider*> adProviders;

    static time_t cooldown;
    static int onlyShowEveryNThAd;
    static int dontShowFirstAdOnAppStart;

    static int adCount;
    static time_t lastAdShownAt;
    static bool enabled;

    static Manager *adManager;

    static void initWithIniFile(const char *iniFile);
    static void startService();

    static void showFullscreenAd();
    static void showBanner();
    static void showBannerIgnoreTime();
    static void showPopup();
    static void openAdLink();

    static bool showNextAd();
    static void hide();
    
    static void enableAds();
    static void disableAds();
    
    template <typename T> static T* getRandomProvider(std::vector<Provider*>)
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

        T *selectedProvider = NULL;
        
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

private:
    Manager() {}
    ~Manager() {}
};

} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_MANAGER_H */