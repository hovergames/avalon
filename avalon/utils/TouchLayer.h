#ifndef AVALON_UTILS_TOUCHLAYER_H
#define AVALON_UTILS_TOUCHLAYER_H

#include "cocos2d.h"
#include <avalon/io/CCBLoader.h>

namespace avalon {
namespace utils {

class TouchLayer : public cocos2d::LayerGradient
{
private:
    cocos2d::EventListenerTouchOneByOne* touchListener = nullptr;

public:
    bool isPressed = false;

    std::function<bool(cocos2d::Touch*, cocos2d::Event*)> onTouchBegan;
    std::function<void(cocos2d::Touch*, cocos2d::Event*)> onTouchMoved;
    std::function<void(cocos2d::Touch*, cocos2d::Event*)> onTouchEnded;
    std::function<void(cocos2d::Touch*, cocos2d::Event*)> onTouchCancelled;

    GLubyte opacityHover = 255;
    GLubyte opacityInactive = 128;

    CREATE_FUNC(TouchLayer);

    virtual bool init() override;

    virtual bool onTouchLayerBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchLayerMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchLayerEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchLayerCancelled(cocos2d::Touch* touch, cocos2d::Event* event);

    void onConfiguration(const avalon::io::CCBLoader::Configuration& config);

    bool isTouchInside(cocos2d::Touch *touch);
    void updateChildOpacity(cocos2d::Node* parent);
};

} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_TOUCHLAYER_H */
