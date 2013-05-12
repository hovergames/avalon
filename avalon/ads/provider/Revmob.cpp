#include <avalon/ads/provider/Revmob.h>

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
    assert("revmob init");
    assert((appId != "") && "appId is empty!");
    
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