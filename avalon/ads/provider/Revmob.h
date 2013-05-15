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
    int weight;
    std::string appId;

    Revmob();

    // Provider
    virtual void init();
    virtual void hideAds();
    virtual int getWeight();

    // Fullscreen
    virtual void showFullscreenAd();

    // Link
    virtual void openAdLink();

    // Banner
    virtual void showBanner();

    // Popup
    virtual void showPopupAd();
};

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_REVMOB_H */
