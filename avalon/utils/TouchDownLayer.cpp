#include "TouchDownLayer.h"

namespace avalon {
namespace utils {

bool TouchDownLayer::onTouchLayerBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (isTouchInside(touch)) {
        if (!isPressed) {
            isPressed = true;
            updateChildOpacity(this);

            if (onTouchBegan)
                onTouchBegan(touch, event);
        }
    }

    return true;
}

void TouchDownLayer::onTouchLayerMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (isTouchInside(touch)) {
        if (!isPressed) {
            isPressed = true;
            updateChildOpacity(this);

            if (onTouchBegan)
                onTouchBegan(touch, event);
        } else {
            if (onTouchMoved)
                onTouchMoved(touch, event);
        }
    } else {
        if (isPressed) {
            isPressed = false;
            updateChildOpacity(this);

            if (onTouchEnded)
                onTouchEnded(touch, event);
        }
    }
}

void TouchDownLayer::onTouchLayerEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (isPressed) {
        isPressed = false;
        updateChildOpacity(this);

        if (onTouchEnded)
            onTouchEnded(touch, event);
    }
}

} // namespace utils
} // namespace avalon