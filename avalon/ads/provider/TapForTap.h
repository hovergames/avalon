#ifndef AVALON_ADS_PROVIDER_TAPFORTAP_H
#define AVALON_ADS_PROVIDER_TAPFORTAP_H

#include <string>
#include <avalon/ads/Provider.h>
#include <avalon/ads/Fullscreen.h>
#include <avalon/ads/Banner.h>
#include <avalon/ads/Link.h>
#include <avalon/ads/Popup.h>

namespace avalon {
namespace ads {
namespace provider {

class TapForTap
: public Fullscreen
, public Provider
, public Banner
{
public:
    std::string apiKey;

    TapForTap();

    // Provider
    virtual void init() override;
    virtual void hideAds() override;

    // Fullscreen
    virtual void showFullscreenAd() override;

    // Banner
    virtual void showBanner() override;    
};

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_TAPFORTAP_H */
