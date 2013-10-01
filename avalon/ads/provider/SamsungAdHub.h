#ifdef AVALON_CONFIG_ADS_PROVIDER_SAMSUNGADHUB_ENABLED

#ifndef AVALON_ADS_PROVIDER_SAMSUNGADHUB_H
#define AVALON_ADS_PROVIDER_SAMSUNGADHUB_H

#include <string>
#include <avalon/ads/Provider.h>
#include <avalon/ads/Fullscreen.h>
#include <avalon/ads/Banner.h>
#include <avalon/ads/Link.h>
#include <avalon/ads/Popup.h>

namespace avalon {
namespace ads {
namespace provider {

class SamsungAdHub
: public Fullscreen
, public Provider
{
public:
    std::string inventoryId;
    
    SamsungAdHub();

    // Provider
    virtual void init() override;
    virtual void hideAds() override;

    // Fullscreen
    virtual void showFullscreenAd() override;
};

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_SAMSUNGADHUB_H */

#endif /* AVALON_CONFIG_ADS_PROVIDER_SAMSUNGADHUB_ENABLED */
