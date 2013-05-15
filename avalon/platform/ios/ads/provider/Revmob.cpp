#include <avalon/ads/provider/Revmob.h>

#include <boost/assert.hpp>
#include <avalon/platform/ios/ads/provider/RevmobBridge.h>

namespace avalon {
namespace ads {
namespace provider {

Revmob::Revmob()
: appId()
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

void Revmob::showFullscreenAd()
{
    RevmobBridge::showFullscreen();
}

void Revmob::openAdLink()
{
    RevmobBridge::openAdLink();
}

void Revmob::showBanner()
{
    RevmobBridge::showBanner();
}

void Revmob::showPopupAd()
{
    RevmobBridge::showPopup();
}

} // namespace provider
} // namespace ads
} // namespace avalon
