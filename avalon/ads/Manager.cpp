#include <avalon/ads/Manager.h>

#include <ctime>
#include "cocos2d.h"
#include <avalon/io/IniReader.h>
#include <avalon/ads/Banner.h>
#include <avalon/ads/Popup.h>
#include <avalon/ads/Link.h>
#include <avalon/ads/provider/Chartboost.h>
#include <avalon/ads/provider/Revmob.h>

using namespace cocos2d;

namespace avalon {
namespace ads {

bool Manager::enabled = true;

time_t Manager::cooldown;
int Manager::onlyShowEveryNThAd = 1;
int Manager::dontShowFirstAdOnAppStart = 0;
int Manager::adCount = 0;

time_t Manager::lastAdShownAt;

Manager *Manager::adManager = NULL;

std::vector<Provider*> Manager::adProviders;

void Manager::initWithIniFile(const char *iniFile)
{
    if (Manager::adManager)
    {
        CCLog("Admanager already initilized");
        return;
    }

    io::IniReader config;
    config.loadFile(iniFile);

    cooldown = config.getValueAsInt("general", "cooldownInMinutes") * 60;        
    onlyShowEveryNThAd = config.getValueAsInt("general", "onlyShowEveryNThAd");
    dontShowFirstAdOnAppStart = config.getValueAsInt("general", "dontShowFirstAdOnAppStart");
    adCount = onlyShowEveryNThAd - 1;

    if (config.doesSectionExist("chartboost"))
    {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        provider::Chartboost *p = new provider::Chartboost();
        p->appId = config.getValue("chartboost", "iosAppId");
        p->appSignature = config.getValue("chartboost", "iosAppSignature");
        p->weight = config.getValueAsInt("chartboost", "weight");
        adProviders.push_back(p);
    #endif
    }

    if (config.doesSectionExist("revmob"))
    {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        provider::Revmob *p = new provider::Revmob();
        p->appId = config.getValue("revmob", "iosAppId");
        p->weight = config.getValueAsInt("revmob", "weight");
        adProviders.push_back(p);
    #endif
    }
}

void Manager::startService()
{
    for (std::vector<Provider*>::iterator it = adProviders.begin(); it != adProviders.end(); ++it)
    {
        (*it)->init();
    }
    lastAdShownAt = 0;
}

void Manager::showFullscreenAd()
{
    Fullscreen *fp = getRandomProvider<Fullscreen>(adProviders);
    if (fp && showNextAd())
    {
        time(&lastAdShownAt);
        fp->showFullscreenAd();
    }
}

void Manager::showBanner()
{
    Banner *bp = getRandomProvider<Banner>(adProviders);
    if (bp && showNextAd())
    {
        time(&lastAdShownAt);
        bp->showBanner();
    }
}

void Manager::showPopup()
{
    Popup *pp = getRandomProvider<Popup>(adProviders);
    if (pp && showNextAd())
    {
        time(&lastAdShownAt);
        pp->showPopupAd();
    }
}

void Manager::openAdLink()
{
    Link *lp = getRandomProvider<Link>(adProviders);
    if (lp && showNextAd())
    {
        time(&lastAdShownAt);
        lp->openAdLink();
    }
}

bool Manager::showNextAd()
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

void Manager::hide()
{
    for (std::vector<Provider*>::iterator it = adProviders.begin(); it != adProviders.end(); ++it)
    {
        (*it)->hideAds();
    }
}

void Manager::enableAds()
{
    enabled = true;
}

void Manager::disableAds()
{
    enabled = false;
}

} // namespace ads
} // namespace avalon