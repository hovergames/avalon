#ifndef AVALON_PHYSICS_NODE_H
#define AVALON_PHYSICS_NODE_H

#include "cocos2d.h"

class b2Body;
namespace avalon { namespace physics { class Box2dContainer; } }

namespace avalon {
namespace physics {

class Node : public cocos2d::Node
{
private:
    Box2dContainer* box2dContainer = nullptr;
    b2Body* body = nullptr;
    float myScaleX = 1;
    float myScaleY = 1;
    
    void removeOldBody();
    
public:
    CREATE_FUNC(Node);
    virtual bool init() override;
    virtual void cleanup() override;
    virtual void update(float delta) override;

    bool hasBody() const;
    b2Body& getBody();
    void setBody(b2Body& body);

    Box2dContainer& getBox2dContainer();
    void setBox2dContainer(Box2dContainer& box2dContainer);

    virtual void setScaleX(float scaleX) override { myScaleX = scaleX; };
    virtual void setScaleY(float scaleY) override { myScaleY = scaleY; };
    virtual void setScale(float scaleX, float scaleY) override { myScaleX = scaleX; myScaleY = scaleY; };
    virtual void setScale(float scale) override { myScaleX = scale; myScaleY = scale; };

    virtual void setPosition(const cocos2d::Point& pos) override { throw std::invalid_argument("NOT ALLOWERD"); };
    virtual void setRotation(float rotation) override { throw std::invalid_argument("NOT ALLOWERD"); };
    virtual void setRotationX(float rotationX) override { throw std::invalid_argument("NOT ALLOWERD"); };
    virtual void setRotationY(float rotationY) override { throw std::invalid_argument("NOT ALLOWERD"); };
    virtual void setPosition(float x, float y) override { throw std::invalid_argument("NOT ALLOWERD"); };
    virtual void setAnchorPoint(const cocos2d::Point& anchor) override { throw std::invalid_argument("NOT ALLOWERD"); };
    virtual void ignoreAnchorPointForPosition(bool value) override { throw std::invalid_argument("NOT ALLOWERD"); };
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_NODE_H */