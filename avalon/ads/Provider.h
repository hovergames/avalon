#ifndef AVALON_ADS_PROVIDER_H
#define AVALON_ADS_PROVIDER_H

namespace avalon {
namespace ads {

class Provider
{
public:
    Provider() : weight(1) {}
    virtual ~Provider() {}

    virtual void init() = 0;
    virtual void hideAds() = 0;

    virtual int getWeight() { return weight; }
    virtual void setWeight(const int newWeight) { weight = newWeight; }

private:
    int weight;
};

} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_H */
