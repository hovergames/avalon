#include "TouchHitLayer.h"

namespace avalon {
namespace utils {

bool TouchHitLayer::onTouchLayerBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (isTouchInside(touch)) {
        return TouchLayer::onTouchLayerBegan(touch, event);
    }

    return false;
}

void TouchHitLayer::onTouchLayerMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (isTouchInside(touch)) {
        TouchLayer::onTouchLayerMoved(touch, event);
    }
}

} // namespace utils
} // namespace avalon