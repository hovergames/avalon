#ifdef AVALON_CONFIG_ADS_PROVIDER_CHARTBOOST_ENABLED

#ifndef AVALON_ADS_PROVIDER_CHARTBOOST_H
#define AVALON_ADS_PROVIDER_CHARTBOOST_H

#include <string>
#include <avalon/ads/Provider.h>
#include <avalon/ads/Fullscreen.h>
#include <avalon/ads/Link.h>

namespace avalon {
namespace ads {
namespace provider {

class Chartboost
: public Fullscreen
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
};

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_CHARTBOOST_H */

#endif /* AVALON_CONFIG_ADS_PROVIDER_CHARTBOOST_ENABLED */
