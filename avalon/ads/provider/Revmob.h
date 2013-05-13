#ifndef AVALON_ADS_PROVIDER_REVMOB_H
#define AVALON_ADS_PROVIDER_REVMOB_H

#include <string>
#include <avalon/ads/Provider.h>
#include <avalon/ads/Fullscreen.h>

namespace avalon {
namespace ads {
namespace provider {

class Revmob
: public Fullscreen
, public Provider
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
};

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_REVMOB_H */
