//
//  Interfaces.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 19.04.13.
//
//

#ifndef __Adventures_on_the_farm__Interfaces__
#define __Adventures_on_the_farm__Interfaces__

namespace Ads
{
    class Link
    {
    public:
        virtual void openAdLink() = 0;
    };

    class Provider
    {
    public:
        virtual void init() = 0;
        virtual void hideAds() = 0;
        virtual int getWeight() = 0;
    };

    class Banner
    {
    public:
        virtual void showBanner() = 0;
    };

    class Fullscreen
    {
    public:
        virtual void showFullscreenAd() = 0;
    };

    class Popup
    {
    public:
        virtual void showPopupAd() = 0;
    };
}

#endif /* defined(__Adventures_on_the_farm__Interfaces__) */
