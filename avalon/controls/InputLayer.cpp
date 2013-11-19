#include <avalon/controls/InputLayer.h>

namespace avalon {
namespace controls {
    
bool InputLayer::init()
{
    if (!cocos2d::LayerGradient::init()) {
        return false;
    }

    touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(InputLayer::onTouchLayerBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(InputLayer::onTouchLayerMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(InputLayer::onTouchLayerEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(InputLayer::onTouchLayerCancelled, this);

    keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(InputLayer::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(InputLayer::onKeyReleased, this);

    auto director = cocos2d::Director::getInstance()->getEventDispatcher();
    director->addEventListenerWithSceneGraphPriority(touchListener, this);
    director->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // let the user configure the appearance and be fully transparent
    // in the default configuration. LayerGradient's defaul is fully visible
    // and both start- and endColor are black. Not the best defaults for our
    // usecase as, most of the time, transparent touch layer ...
    setOpacity(0);

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
    if (activeKeys.size() > 0 || activeTouchIds.size() > 0) {
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

    activeTouchIds.insert(touch->getID());
    if (pressed) {
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

    if (!pressed) {
        activeTouchIds.insert(touch->getID());
    }
    pressed = true;

    if (!activeTouchIds.count(touch->getID())) {
        return;
    }

    if (onTouchMoved) {
        onTouchMoved(touch, event);
    }
}

void InputLayer::onTouchLayerEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!activeTouchIds.count(touch->getID())) {
        return;
    }

    if (isFilteredEnded(touch, event)) {
        return;
    }

    activeTouchIds.erase(touch->getID());
    if (activeKeys.size() > 0 || activeTouchIds.size() > 0) {
        return;
    }

    pressed = false;
    if (onTouchEnded) {
        onTouchEnded(touch, event);
    }
}

void InputLayer::onTouchLayerCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if (!activeTouchIds.count(touch->getID())) {
        return;
    }

    if (isFilteredCancelled(touch, event)) {
        return;
    }

    pressed = false;
    activeKeys.clear();
    activeTouchIds.clear();

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

} // namespace controls
} // namespace avalon