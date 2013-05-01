//
//  AdManager.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 19.04.13.
//
//

#include "AdManager.h"
#include "IniReader.h"
#include "ChartboostProvider.h"
#include "RevmobProvider.h"
#include "cocos2d.h"
#include <ctime>

using namespace cocos2d;

namespace Ads
{
    bool AdManager::enabled = true;
    
    time_t AdManager::cooldown;
    int AdManager::onlyShowEveryNThAd = 1;
    int AdManager::dontShowFirstAdOnAppStart = 0;
    int AdManager::adCount = 0;

    time_t AdManager::lastAdShownAt;
    
    AdManager *AdManager::adManager = NULL;

    std::vector<Provider*> AdManager::adProviders;
    
    void AdManager::initWithIniFile(const char *iniFile)
    {
        if (AdManager::adManager)
        {
            CCLog("Admanager already initilized");
            return;
        }

        IniReader config;
        config.loadFile(iniFile);

        cooldown = config.getValueAsInt("general", "cooldownInMinutes") * 60;        
        onlyShowEveryNThAd = config.getValueAsInt("general", "onlyShowEveryNThAd");
        dontShowFirstAdOnAppStart = config.getValueAsInt("general", "dontShowFirstAdOnAppStart");
        adCount = onlyShowEveryNThAd - 1;

        if (config.doesSectionExist("chartboost"))
        {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            ChartboostProvider *p = new ChartboostProvider();
            p->appId = config.getValue("chartboost", "iosAppId");
            p->appSignature = config.getValue("chartboost", "iosAppSignature");
            p->weight = config.getValueAsInt("chartboost", "weight");
            adProviders.push_back(p);
        #endif
        }

        if (config.doesSectionExist("revmob"))
        {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            RevmobProvider *p = new RevmobProvider();
            p->appId = config.getValue("revmob", "iosAppId");
            p->weight = config.getValueAsInt("revmob", "weight");
            adProviders.push_back(p);
        #endif
        }
    }

    void AdManager::startService()
    {
        for (std::vector<Provider*>::iterator it = adProviders.begin(); it != adProviders.end(); ++it)
        {
            (*it)->init();
        }
        lastAdShownAt = 0;
    }

    void AdManager::showFullscreenAd()
    {
        Fullscreen *fp = getRandomProvider<Fullscreen>(adProviders);
        if (fp && showNextAd())
        {
            time(&lastAdShownAt);
            fp->showFullscreenAd();
        }
    }

    void AdManager::showBanner()
    {
        Banner *bp = getRandomProvider<Banner>(adProviders);
        if (bp && showNextAd())
        {
            time(&lastAdShownAt);
            bp->showBanner();
        }
    }

    void AdManager::showPopup()
    {
        Popup *pp = getRandomProvider<Popup>(adProviders);
        if (pp && showNextAd())
        {
            time(&lastAdShownAt);
            pp->showPopupAd();
        }
    }

    void AdManager::openAdLink()
    {
        Link *lp = getRandomProvider<Link>(adProviders);
        if (lp && showNextAd())
        {
            time(&lastAdShownAt);
            lp->openAdLink();
        }
    }

    bool AdManager::showNextAd()
    {
        if (!enabled)
            return false;

        if (dontShowFirstAdOnAppStart == 1)
        {
            dontShowFirstAdOnAppStart = 0;
            return false;
        }

        ++adCount;

        if (onlyShowEveryNThAd > 0 && (adCount % onlyShowEveryNThAd) != 0)
            return false;

        time_t now;
        time(&now);
        
        if ((lastAdShownAt + cooldown) > now)
            return false;

        return true;
    }

    void AdManager::hide()
    {
        for (std::vector<Provider*>::iterator it = adProviders.begin(); it != adProviders.end(); ++it)
        {
            (*it)->hideAds();
        }
    }

    void AdManager::enableAds()
    {
        enabled = true;
    }

    void AdManager::disableAds()
    {
        enabled = false;
    }
}