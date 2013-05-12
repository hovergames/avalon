#ifndef AVALON_ADS_PROVIDER_H
#define AVALON_ADS_PROVIDER_H

namespace avalon {
namespace ads {

class Provider
{
public:
    virtual void init() = 0;
    virtual void hideAds() = 0;
    virtual int getWeight() = 0;
};

} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_H */
