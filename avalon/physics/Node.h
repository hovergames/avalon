#ifndef AVALON_PHYSICS_NODE_H
#define AVALON_PHYSICS_NODE_H

#include "cocos2d.h"
#include <avalon/physics/Body.h>

#define CREATE_FUNC_PHYSICAL(__TYPE__) \
    static __TYPE__* create(avalon::physics::Box2dContainer& box2dContainer, b2Body& body) \
    { \
        __TYPE__ *result = new __TYPE__(); \
        if (result && result->init(box2dContainer, body)) { \
            result->autorelease(); \
            return result; \
        } \
        delete result; \
        return nullptr; \
    }

namespace avalon {
namespace physics {

class Node : public cocos2d::Node
{
private:
    std::shared_ptr<Body> bodyImpl;

    void syncBody();
    
public:
    CREATE_FUNC_PHYSICAL(Node);
    virtual bool init(avalon::physics::Box2dContainer& box2dContainer, b2Body& body);

    virtual void cleanup() override;
    virtual void update(float delta) override;

    // Redirect all important methods
    b2Body& getBody()                                                  { return bodyImpl->getBody(); }
    Box2dContainer& getBox2dContainer()                                { return bodyImpl->getBox2dContainer(); }
    virtual void setScaleX(float scaleX) override                      { bodyImpl->setScaleX(scaleX); }
    virtual void setScaleY(float scaleY) override                      { bodyImpl->setScaleY(scaleY); }
    virtual void setScale(float scaleX, float scaleY) override         { bodyImpl->setScale(scaleX, scaleY); }
    virtual void setScale(float scale) override                        { bodyImpl->setScale(scale); }
    virtual void setPosition(const cocos2d::Point& pos) override       { bodyImpl->setPosition(pos); }
    virtual void setPosition(float x, float y) override                { bodyImpl->setPosition(x, y); }
    virtual void setRotation(float rotation) override                  { bodyImpl->setRotation(rotation); }
    virtual void setRotationX(float rotationX) override                { bodyImpl->setRotationX(rotationX); }
    virtual void setRotationY(float rotationY) override                { bodyImpl->setRotationY(rotationY); }
    virtual void setAnchorPoint(const cocos2d::Point& anchor) override { bodyImpl->setAnchorPoint(anchor); }
    virtual void ignoreAnchorPointForPosition(bool value) override     { bodyImpl->ignoreAnchorPointForPosition(value); }
    void setPositionOffset(const cocos2d::Point& point)                { bodyImpl->setPositionOffset(point); }
    void setRotationOffset(float rotation)                             { bodyImpl->setRotationOffset(rotation); }
    const cocos2d::Point& getPositionOffset()                          { return bodyImpl->getPositionOffset(); }
    float getRotationOffset()                                          { return bodyImpl->getRotationOffset(); }
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_NODE_H */