#ifndef AVALON_GRAPHICS_PARALLAXCONTAINER_H
#define AVALON_GRAPHICS_PARALLAXCONTAINER_H

#include "cocos2d.h"

namespace avalon {
namespace graphics {

class  ParallaxContainer : public cocos2d::Node
{
public:
    CREATE_FUNC(ParallaxContainer);

    void addChild(Node * child, int z, const cocos2d::Vec2& parallaxRatio, const cocos2d::Vec2& positionOffset);

    void addChild(Node * child, int zOrder, int tag) override;
    void removeChild(Node* child, bool cleanup) override;
    void removeAllChildrenWithCleanup(bool cleanup) override;
    void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, bool parentTransformUpdated) override;

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
