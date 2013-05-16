#ifndef AVALON_ADS_PROVIDER_REVMOB_H
#define AVALON_ADS_PROVIDER_REVMOB_H

#include <string>
#include <avalon/ads/Provider.h>
#include <avalon/ads/Fullscreen.h>
#include <avalon/ads/Banner.h>
#include <avalon/ads/Link.h>
#include <avalon/ads/Popup.h>

namespace avalon {
namespace ads {
namespace provider {

class Revmob
: public Fullscreen
, public Provider
, public Banner
, public Link
, public Popup
{
public:
    std::string appId;
    
    Revmob();

    // Provider
    virtual void init() override;
    virtual void hideAds() override;

    // Fullscreen
    virtual void showFullscreenAd() override;

    // Link
    virtual void openAdLink() override;

    // Banner
    virtual void showBanner() override;

    // Popup
    virtual void showPopupAd() override;
};

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_REVMOB_H */
