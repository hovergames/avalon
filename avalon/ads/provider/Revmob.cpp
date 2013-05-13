#include <avalon/ads/provider/Revmob.h>

#include <boost/assert.hpp>
#include <avalon/ads/provider/RevmobBridge.h>

namespace avalon {
namespace ads {
namespace provider {

Revmob::Revmob()
: weight(1)
, appId()
{
}

void Revmob::init()
{
    BOOST_ASSERT_MSG(appId != "", "appId must be set first");

    RevmobBridge::startSession(appId.c_str());
}

void Revmob::hideAds()
{
    RevmobBridge::hideBanner();
}

int Revmob::getWeight()
{
    return weight;
}

void Revmob::showFullscreenAd()
{
    RevmobBridge::showFullscreen();
}

} // namespace provider
} // namespace ads
} // namespace avalon