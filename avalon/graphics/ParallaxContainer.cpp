#include "ParallaxContainer.h"

using namespace cocos2d;

namespace avalon {
namespace graphics {

class PointObject : public Ref
{
public:
    static PointObject * create(Vec2 ratio, Vec2 offset)
    {
        PointObject *ret = new PointObject();
        ret->initWithPoint(ratio, offset);
        ret->autorelease();
        return ret;
    }

    bool initWithPoint(Vec2 ratio, Vec2 offset)
    {
        _ratio = ratio;
        _offset = offset;
        _child = nullptr;
        return true;
    }

    inline const Vec2& getRatio() const { return _ratio; };
    inline void setRatio(const Vec2& ratio) { _ratio = ratio; };

    inline const Vec2& getOffset() const { return _offset; };
    inline void setOffset(const Vec2& offset) { _offset = offset; };

    inline Node* getChild() const { return _child; };
    inline void setChild(Node* child) { _child = child; };

private:
    Vec2 _ratio;
    Vec2 _offset;
    Node *_child; // weak ref
};

ParallaxContainer::ParallaxContainer()
{
    _parallaxArray = ccArrayNew(5);
    _lastPosition = Vec2(-100,-100);
}

ParallaxContainer::~ParallaxContainer()
{
    if (_parallaxArray) {
        ccArrayFree(_parallaxArray);
        _parallaxArray = nullptr;
    }
}

void ParallaxContainer::addChild(Node * child, int zOrder, int tag)
{
    CC_UNUSED_PARAM(zOrder);
    CC_UNUSED_PARAM(child);
    CC_UNUSED_PARAM(tag);
    CCASSERT(0,"ParallaxNode: use addChild:z:parallaxRatio:positionOffset instead");
}

void ParallaxContainer::addChild(Node *child, int z, const Vec2& ratio, const Vec2& offset)
{
    CCASSERT( child != nullptr, "Argument must be non-nil");
    PointObject *obj = PointObject::create(ratio, offset);
    obj->setChild(child);
    ccArrayAppendObjectWithResize(_parallaxArray, (Ref*)obj);

    Node::addChild(child, z, child->getTag());
}

void ParallaxContainer::removeChild(Node* child, bool cleanup)
{
    for( int i=0;i < _parallaxArray->num;i++) {
        PointObject *point = (PointObject*)_parallaxArray->arr[i];
        if (point->getChild() == child) {
            ccArrayRemoveObjectAtIndex(_parallaxArray, i, true);
            break;
        }
    }
    Node::removeChild(child, cleanup);
}

void ParallaxContainer::removeAllChildrenWithCleanup(bool cleanup)
{
    ccArrayRemoveAllObjects(_parallaxArray);
    Node::removeAllChildrenWithCleanup(cleanup);
}

void ParallaxContainer::visit(Renderer *renderer, const Mat4 &parentTransform, bool parentTransformUpdated)
{
    Vec2 pos = parallaxPosition;

    if (!pos.equals(_lastPosition)) {
        for( int i=0; i < _parallaxArray->num; i++ ) {
            PointObject *point = (PointObject*)_parallaxArray->arr[i];
            float x = pos.x * point->getRatio().x + point->getOffset().x;
            float y = pos.y * point->getRatio().y + point->getOffset().y;
            point->getChild()->setPosition({x, y});
        }
        _lastPosition = pos;
    }
    Node::visit(renderer, parentTransform, parentTransformUpdated);
}

} // namespace graphics
} // namespace avalon