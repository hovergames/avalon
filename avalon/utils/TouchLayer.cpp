#include "TouchLayer.h"
#include "cocos2d.h"

namespace avalon {
namespace utils {

bool TouchLayer::init()
{
    if (!cocos2d::LayerGradient::init())
        return false;

    touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = std::bind(&TouchLayer::onTouchLayerBegan, this, std::placeholders::_1, std::placeholders::_2);
    touchListener->onTouchMoved = std::bind(&TouchLayer::onTouchLayerMoved, this, std::placeholders::_1, std::placeholders::_2);
    touchListener->onTouchEnded = std::bind(&TouchLayer::onTouchLayerEnded, this, std::placeholders::_1, std::placeholders::_2);
    touchListener->onTouchCancelled = std::bind(&TouchLayer::onTouchLayerCancelled, this, std::placeholders::_1, std::placeholders::_2);

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool TouchLayer::onTouchLayerBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (onTouchBegan)
        onTouchBegan(touch, event);

    pressed = true;
    updateChildOpacity(this);

    return true;
}

void TouchLayer::onTouchLayerMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (onTouchMoved)
        onTouchMoved(touch, event);

    pressed = true;
    updateChildOpacity(this);
}

void TouchLayer::onTouchLayerEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (onTouchEnded)
        onTouchEnded(touch, event);

    pressed = false;
    updateChildOpacity(this);
}

void TouchLayer::onTouchLayerCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (onTouchCancelled)
        onTouchCancelled(touch, event);

    pressed = false;
    updateChildOpacity(this);
}

bool TouchLayer::isTouchInside(cocos2d::Touch *touch)
{
    cocos2d::Point touchLocation = touch->getLocation();
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
    cocos2d::Rect bBox = getBoundingBox();
    return bBox.containsPoint(touchLocation);
}

void TouchLayer::updateChildOpacity(cocos2d::Node* parent)
{
    GLubyte opacity = opacityInactive;
    if (pressed) opacity = opacityHover;

    cocos2d::Object* element;
    CCARRAY_FOREACH(getChildren(), element) {
        cocos2d::Sprite* sprite = dynamic_cast<cocos2d::Sprite*>(element);
        if (sprite) {
            sprite->setOpacity(opacity);
            if (sprite->getChildren())
                updateChildOpacity(sprite);
        }
    }
};

void TouchLayer::onConfiguration(const avalon::io::CCBLoader::Configuration& config)
{
}

bool TouchLayer::isPressed()
{
    return pressed;
}

} // namespace utils
} // namespace avalon