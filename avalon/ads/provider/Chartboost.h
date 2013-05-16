#ifndef AVALON_ADS_PROVIDER_CHARTBOOST_H
#define AVALON_ADS_PROVIDER_CHARTBOOST_H

#include <string>
#include <avalon/ads/Provider.h>
#include <avalon/ads/Fullscreen.h>
#include <avalon/ads/Link.h>
#include <avalon/ads/provider/ChartboostX.h>

namespace avalon {
namespace ads {
namespace provider {

class Chartboost
: public ChartboostXDelegate
, public Fullscreen
, public Provider
, public Link
{
public:
    std::string appId;
    std::string appSignature;

    Chartboost();

    // Provider
    virtual void init() override;
    virtual void hideAds() override;

    // Fullscreen
    virtual void showFullscreenAd() override;

    // Link
    virtual void openAdLink() override;

    // ChartboostXDelegate method
    virtual bool shouldDisplayInterstitial(const char* location) override;
    virtual void didCacheInterstitial(const char* location) override;
    virtual void didFailToLoadInterstitial(const char* location) override;
    virtual void didCloseInterstitial(const char* location) override;
    virtual void didClickInterstitial(const char* location) override;
};

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_CHARTBOOST_H */
