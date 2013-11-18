#ifndef AVALON_CONTROLS_INPUTLAYER_H
#define AVALON_CONTROLS_INPUTLAYER_H

#include "cocos2d.h"
#include "editor-support/cocosbuilder/CocosBuilder.h"
#include <avalon/io/CCBLoader.h>

namespace avalon {
namespace controls {

class InputLayer : public cocos2d::LayerGradient
{
private:
    cocos2d::EventListenerTouchOneByOne* touchListener = nullptr;
    cocos2d::EventListenerKeyboard* keyboardListener = nullptr;
    std::set<cocos2d::EventKeyboard::KeyCode> listenKeys;
    std::set<cocos2d::EventKeyboard::KeyCode> activeKeys;
    std::set<int> activeTouchIds;

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

protected:
    bool pressed = false;
    virtual bool isFilteredBegan(cocos2d::Touch* touch, cocos2d::Event* event) { return false; }
    virtual bool isFilteredMoved(cocos2d::Touch* touch, cocos2d::Event* event) { return false; }
    virtual bool isFilteredEnded(cocos2d::Touch* touch, cocos2d::Event* event) { return false; }
    virtual bool isFilteredCancelled(cocos2d::Touch* touch, cocos2d::Event* event) { return false; }

public:
    using CCBLoader = cocosbuilder::LayerGradientLoader;
    
    std::function<bool(cocos2d::Touch*, cocos2d::Event*)> onTouchBegan;
    std::function<void(cocos2d::Touch*, cocos2d::Event*)> onTouchMoved;
    std::function<void(cocos2d::Touch*, cocos2d::Event*)> onTouchEnded;
    std::function<void(cocos2d::Touch*, cocos2d::Event*)> onTouchCancelled;

    bool onTouchLayerBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchLayerMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchLayerEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchLayerCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

    CREATE_FUNC(InputLayer);
    virtual bool init() override;

    bool isPressed() const;
    bool isTouchInside(const cocos2d::Touch& touch) const;

    void onConfiguration(const avalon::io::CCBLoader::Configuration& config);
    void bindKey(const cocos2d::EventKeyboard::KeyCode keyCode);
};

} // namespace controls
} // namespace avalon

#endif /* AVALON_CONTROLS_INPUTLAYER_H */
