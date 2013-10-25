#ifdef AVALON_CONFIG_ADS_PROVIDER_IAD_ENABLED

#ifndef AVALON_ADS_PROVIDER_IAD_H
#define AVALON_ADS_PROVIDER_IAD_H

#include <string>
#include <avalon/ads/Provider.h>
#include <avalon/ads/Banner.h>

namespace avalon {
namespace ads {
namespace provider {

class IAd
: public Provider
, public Banner
{
public:
    IAd();

    // Provider
    virtual void init() override;
    virtual void hideAds() override;

    // Banner
    virtual void showBanner() override;
};

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_IAD_H */

#endif /* AVALON_CONFIG_ADS_PROVIDER_IAD_ENABLED */