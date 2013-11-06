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

    void removeOldBody();
    
public:
    CREATE_FUNC(Node);
    virtual bool init() override;
    virtual void cleanup() override;
    virtual void update(float delta) override;

    b2Body& getBody();
    void setBody(b2Body& body);

    Box2dContainer& getBox2dContainer();
    void setBox2dContainer(Box2dContainer& box2dContainer);
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_NODE_H */