#include <avalon/controls/InputLayer.h>

namespace avalon {
namespace controls {
    
bool InputLayer::init()
{
    if (!cocos2d::LayerGradient::init()) {
        return false;
    }

    using namespace std::placeholders;

    touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = std::bind(&InputLayer::onTouchLayerBegan, this, _1, _2);
    touchListener->onTouchMoved = std::bind(&InputLayer::onTouchLayerMoved, this, _1, _2);
    touchListener->onTouchEnded = std::bind(&InputLayer::onTouchLayerEnded, this, _1, _2);
    touchListener->onTouchCancelled = std::bind(&InputLayer::onTouchLayerCancelled, this, _1, _2);

    keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = std::bind(&InputLayer::onKeyPressed, this, _1, _2);
    keyboardListener->onKeyReleased = std::bind(&InputLayer::onKeyReleased, this, _1, _2);

    auto director = cocos2d::Director::getInstance()->getEventDispatcher();
    director->addEventListenerWithSceneGraphPriority(touchListener, this);
    director->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    return true;
}

void InputLayer::bindKey(const cocos2d::EventKeyboard::KeyCode keyCode)
{
    listenKeys.insert(keyCode);
}

void InputLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if (!listenKeys.count(keyCode)) {
        return;
    }

    activeKeys.insert(keyCode);
    if (pressed) {
        return;
    }

    pressed = true;
    if (onTouchBegan) {
        onTouchBegan(nullptr, event);
    }
}

void InputLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if (!listenKeys.count(keyCode)) {
        return;
    }

    activeKeys.erase(keyCode);
    if (activeKeys.size() > 0) {
        return;
    }

    pressed = false;
    if (onTouchEnded) {
        onTouchEnded(nullptr, event);
    }
}

bool InputLayer::onTouchLayerBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (isFilteredBegan(touch, event)) {
        return true;
    }

    pressed = true;
    if (onTouchBegan) {
        return onTouchBegan(touch, event);
    } else {
        return true;
    }
}

void InputLayer::onTouchLayerMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (isFilteredMoved(touch, event)) {
        return;
    }

    pressed = true;
    if (onTouchMoved) {
        onTouchMoved(touch, event);
    }
}

void InputLayer::onTouchLayerEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (activeKeys.size() > 0) {
        return;
    }

    if (isFilteredEnded(touch, event)) {
        return;
    }

    pressed = false;
    if (onTouchEnded) {
        onTouchEnded(touch, event);
    }
}

void InputLayer::onTouchLayerCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (isFilteredCancelled(touch, event)) {
        return;
    }

    pressed = false;
    activeKeys.clear();

    if (onTouchCancelled) {
        onTouchCancelled(touch, event);
    }
}

bool InputLayer::isTouchInside(const cocos2d::Touch& touch) const
{
    cocos2d::Point touchLocation = touch.getLocation();
    touchLocation = getParent()->convertToNodeSpace(touchLocation);
    return getBoundingBox().containsPoint(touchLocation);
}

bool InputLayer::isPressed() const
{
    return pressed;
}

void InputLayer::onConfiguration(const avalon::io::CCBLoader::Configuration& config)
{
}

void InputLayer::updateChildOpacity(cocos2d::Node& parent) const
{
    GLubyte opacity = opacityInactive;
    if (pressed) {
        opacity = opacityHover;
    }

    cocos2d::Object* element = nullptr;
    CCARRAY_FOREACH(getChildren(), element) {
        cocos2d::Sprite* sprite = dynamic_cast<cocos2d::Sprite*>(element);
        if (!sprite) {
            continue;
        }

        sprite->setOpacity(opacity);
        if (sprite->getChildren()) {
            updateChildOpacity(*sprite);
        }
    }
};

} // namespace controls
} // namespace avalon