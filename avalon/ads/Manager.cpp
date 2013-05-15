#include <avalon/ads/Manager.h>

#include <ctime>
#include "cocos2d.h"
#include <avalon/io/IniReader.h>
#include <avalon/ads/Banner.h>
#include <avalon/ads/Popup.h>
#include <avalon/ads/Link.h>
#include <avalon/ads/provider/Chartboost.h>
#include <avalon/ads/provider/Revmob.h>
#include <avalon/utils/platform.h>

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
    if (Manager::adManager) {
        CCLog("Admanager already initilized");
        return;
    }

    io::IniReader config;
    config.loadFile(iniFile);

    cooldown = config.getValueAsInt("general", "cooldownInMinutes") * 60;
    onlyShowEveryNThAd = config.getValueAsInt("general", "onlyShowEveryNThAd");
    dontShowFirstAdOnAppStart = config.getValueAsInt("general", "dontShowFirstAdOnAppStart");
    adCount = onlyShowEveryNThAd - 1;

    auto flavor = avalon::utils::platform::getFlavor();
    flavor[0] = std::toupper(flavor[0]);
    auto prefix = avalon::utils::platform::getName() + flavor;

    if (config.doesSectionExist("chartboost")) {
        provider::Chartboost *p = new provider::Chartboost();
        p->setWeight(config.getValueAsInt("chartboost", "weight"));
        p->appId = config.getValue("chartboost", (prefix + "AppId").c_str());
        p->appSignature = config.getValue("chartboost", (prefix + "AppSignature").c_str());
        adProviders.push_back(p);
    }

    if (config.doesSectionExist("revmob")) {
        provider::Revmob *p = new provider::Revmob();
        p->setWeight(config.getValueAsInt("revmob", "weight"));
        p->appId = config.getValue("revmob", (prefix + "AppId").c_str());
        adProviders.push_back(p);
    }
}

void Manager::startService()
{
    for (auto provider : adProviders) {
        provider->init();
    }
    lastAdShownAt = 0;
}

void Manager::showFullscreenAd()
{
    Fullscreen *fp = getRandomProvider<Fullscreen>(adProviders);
    if (fp && showNextAd()) {
        time(&lastAdShownAt);
        fp->showFullscreenAd();
    }
}

void Manager::showBanner()
{
    Banner *bp = getRandomProvider<Banner>(adProviders);
    if (bp && showNextAd()) {
        time(&lastAdShownAt);
        bp->showBanner();
    }
}

void Manager::showPopup()
{
    Popup *pp = getRandomProvider<Popup>(adProviders);
    if (pp && showNextAd()) {
        time(&lastAdShownAt);
        pp->showPopupAd();
    }
}

void Manager::openAdLink()
{
    Link *lp = getRandomProvider<Link>(adProviders);
    if (lp && showNextAd()) {
        time(&lastAdShownAt);
        lp->openAdLink();
    }
}

bool Manager::showNextAd()
{
    if (!enabled) {
        return false;
    }

    if (dontShowFirstAdOnAppStart == 1) {
        dontShowFirstAdOnAppStart = 0;
        return false;
    }

    ++adCount;

    if (onlyShowEveryNThAd > 0 && (adCount % onlyShowEveryNThAd) != 0) {
        return false;
    }

    time_t now;
    time(&now);

    if ((lastAdShownAt + cooldown) > now) {
        return false;
    }

    return true;
}

void Manager::hide()
{
    for (auto provider : adProviders) {
        provider->hideAds();
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
