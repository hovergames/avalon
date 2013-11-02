#ifndef AVALON_UTILS_TOUCHDOWNLAYER_H
#define AVALON_UTILS_TOUCHDOWNLAYER_H

#include "cocos2d.h"
#include <avalon/io/CCBLoader.h>
#include <avalon/utils/TouchLayer.h>

namespace avalon {
namespace utils {

class TouchDownLayer : public TouchLayer
{
public:
    CREATE_FUNC(TouchDownLayer);

    virtual bool onTouchLayerBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchLayerMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchLayerEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};


} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_TOUCHDOWNLAYER_H */
