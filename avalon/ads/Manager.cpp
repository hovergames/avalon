#include <avalon/ads/Manager.h>

#include <ctime>
#include "cocos2d.h"
#include <avalon/io/IniReader.h>
#include <avalon/ads/Banner.h>
#include <avalon/ads/Popup.h>
#include <avalon/ads/Link.h>
#include <avalon/utils/platform.h>

#ifdef AVALON_ENABLE_ADS_PROVIDER_SAMSUNGADHUB
    #include <avalon/ads/provider/SamsungAdHub.h>
#endif
#if AVALON_PLATFORM_IS_ANDROID_SAMSUNG
    // Only Samsung AdHub is allowed on the Samsung Apps store ...
#else
    #include <avalon/ads/provider/Chartboost.h>
    #include <avalon/ads/provider/Revmob.h>
#endif
#if AVALON_PLATFORM_IS_IOS || AVALON_PLATFORM_IS_ANDROID_GOOGLE
    #include <avalon/ads/provider/TapForTap.h>
#endif
#if AVALON_PLATFORM_IS_IOS
    #include <avalon/ads/provider/IAd.h>
#endif

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
bool Manager::bannerVisible = false;

void Manager::initWithIniFile(const char *iniFile)
{
    if (Manager::adManager) {
        log("Admanager already initilized");
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

#ifdef AVALON_ENABLE_ADS_PROVIDER_SAMSUNGADHUB
    if (config.doesSectionExist("samsungadhub")) {
        auto *p = new provider::SamsungAdHub();
        p->setWeight(config.getValueAsInt("samsungadhub", "weight"));
        p->inventoryId = config.getValue("samsungadhub", (prefix + "InventoryId").c_str());
        adProviders.push_back(p);
    }
#endif
    
#if AVALON_PLATFORM_IS_ANDROID_SAMSUNG
    // Only Samsung AdHub is allowed on the Samsung Apps store ...
#else
    if (config.doesSectionExist("chartboost")) {
        auto *p = new provider::Chartboost();
        p->setWeight(config.getValueAsInt("chartboost", "weight"));
        p->appId = config.getValue("chartboost", (prefix + "AppId").c_str());
        p->appSignature = config.getValue("chartboost", (prefix + "AppSignature").c_str());
        adProviders.push_back(p);
    }

    if (config.doesSectionExist("revmob")) {
        auto *p = new provider::Revmob();
        p->setWeight(config.getValueAsInt("revmob", "weight"));
        p->appId = config.getValue("revmob", (prefix + "AppId").c_str());
        adProviders.push_back(p);
    }
#endif

#if AVALON_PLATFORM_IS_IOS || AVALON_PLATFORM_IS_ANDROID_GOOGLE
    if (config.doesSectionExist("tapfortap")) {
        provider::TapForTap *p = new provider::TapForTap();
        p->setWeight(config.getValueAsInt("tapfortap", "weight"));
        p->apiKey = config.getValue("tapfortap", "apiKey");
        adProviders.push_back(p);
    }
#endif

#if AVALON_PLATFORM_IS_IOS && AVALON_ENABLE_ADS_PROVIDER_IAD
    if (config.doesSectionExist("iad")) {
        provider::IAd *p = new provider::IAd();
        p->setWeight(config.getValueAsInt("iad", "weight"));
        adProviders.push_back(p);
    }
#endif
}

void Manager::startService()
{
    for (auto& provider : adProviders) {
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
    if (bannerVisible) {
        return;
    }

    Banner *bp = getRandomProvider<Banner>(adProviders);
    if (bp && showNextAd()) {
        time(&lastAdShownAt);
        bp->showBanner();
        bannerVisible = true;
    }
}

void Manager::showBannerIgnoreTime()
{
    if (!enabled || bannerVisible) {
        return;
    }
    
    Banner *bp = getRandomProvider<Banner>(adProviders);
    if (bp) {
        bp->showBanner();
        bannerVisible = true;
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
    for (auto& provider : adProviders) {
        provider->hideAds();
    }
    bannerVisible = false;
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
