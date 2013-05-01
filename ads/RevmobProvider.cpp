//
//  RevmobProvider.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 19.04.13.
//
//

#include "RevmobProvider.h"

namespace Ads
{
    RevmobProvider::RevmobProvider() : weight(1)
    {
    }
    
    void RevmobProvider::init()
    {
        assert("revmob init");
        assert((appId != "") && "appId is empty!");
        RevmobBridge::startSession(appId.c_str());
    }

    void RevmobProvider::hideAds()
    {
        RevmobBridge::hideBanner();
    }

    int RevmobProvider::getWeight()
    {
        return weight;
    }

    void RevmobProvider::showFullscreenAd()
    {
        RevmobBridge::showFullscreen();
    }
}