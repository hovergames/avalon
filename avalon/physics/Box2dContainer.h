#ifndef AVALON_PHYSICS_BOX2DCONTAINER_H
#define AVALON_PHYSICS_BOX2DCONTAINER_H

#include "Box2D/Box2D.h"
#include "cocos2d.h"

class B2DebugDrawLayer;

namespace avalon {
namespace physics {

class Box2dContainer : public cocos2d::Node
{
private:
    using NodeId = unsigned int;

    NodeId lastId = 0;
    B2DebugDrawLayer* debugLayer = nullptr;
    std::unordered_map<NodeId, cocos2d::Node*> idToNode;
    std::unordered_map<cocos2d::Node*, NodeId> nodeToId;

    NodeId generateId();

public:
    std::shared_ptr<b2World> world;
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    float pixelsInMeter = 100; // todo: read only

    CREATE_FUNC(Box2dContainer);
    ~Box2dContainer();
    bool init() override;

    void update(float delta) override;
    void enableDebugDraw(const bool enable);

    b2Body* createBody(const b2BodyDef& bodyDef);
    b2Body* createBody(const b2BodyDef& bodyDef, cocos2d::Node& bode);

    template<typename T>
    T* getNode(const b2Body& body);
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_BOX2DCONTAINER_H */