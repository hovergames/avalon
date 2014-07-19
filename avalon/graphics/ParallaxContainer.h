#ifndef AVALON_GRAPHICS_PARALLAXCONTAINER_H
#define AVALON_GRAPHICS_PARALLAXCONTAINER_H

#include "cocos2d.h"

namespace avalon {
namespace graphics {

class  ParallaxContainer : public cocos2d::Node
{
public:
    CREATE_FUNC(ParallaxContainer);

    cocos2d::Vec2 parallaxPosition = {0.0, 0.0};

    void addChild(Node * child, int z, const cocos2d::Vec2& parallaxRatio, const cocos2d::Vec2& positionOffset);

    void addChild(Node * child, int zOrder, int tag) override;
    void removeChild(Node* child, bool cleanup) override;
    void removeAllChildrenWithCleanup(bool cleanup) override;
    void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

    inline void setParallaxX(float x) { parallaxPosition.x = x; }
    inline void setParallaxY(float y) { parallaxPosition.y = y; }
    inline void setParallaxPosition(float x, float y) { setParallaxX(x); setParallaxY(y); }

    inline float getParallaxX() { return parallaxPosition.x; }
    inline float getParallaxY() { return parallaxPosition.y; }
    inline cocos2d::Vec2 getParallaxPosition() const { return parallaxPosition; }

protected:
    ParallaxContainer();
    virtual ~ParallaxContainer();

    cocos2d::Vec2    _lastPosition;
    struct cocos2d::_ccArray* _parallaxArray;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(ParallaxContainer);
};


} // namespace graphics
} // namespace avalon

#endif /* defined(AVALON_GRAPHICS_PARALLAXCONTAINER_H) */
