//
//  RevmobProvider.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 19.04.13.
//
//

#ifndef __Adventures_on_the_farm__RevmobProvider__
#define __Adventures_on_the_farm__RevmobProvider__

#include "Interfaces.h"
#include "RevmobBridge.h"
#include <string>
namespace Ads
{
    class RevmobProvider : public Ads::Fullscreen, public Ads::Provider
    {
    public:
        int weight;

        RevmobProvider();

        std::string appId;

        // Ads::Provider
        virtual void init();
        virtual void hideAds();
        virtual int getWeight();

        // Ads::Fullscreen
        virtual void showFullscreenAd();
    };
}
#endif /* defined(__Adventures_on_the_farm__RevmobProvider__) */
