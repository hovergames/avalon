#ifndef AVALON_GRAPHICS_CLIPPINGNODERECT_H
#define AVALON_GRAPHICS_CLIPPINGNODERECT_H

#include "cocos2d.h"

namespace avalon {
namespace graphics {

class ClippingNodeRect : public cocos2d::Node
{
public:
    CREATE_FUNC(ClippingNodeRect);
    
    void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated) override;
};

} // graphics
} // avalon

#endif // AVALON_GRAPHICS_CLIPPINGNODERECT_H