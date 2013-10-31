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
    B2DebugDrawLayer* debugLayer = nullptr;

public:
    std::shared_ptr<b2World> world;
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    float pixelsInMeter = 100; // todo: read only

    CREATE_FUNC(Box2dContainer);
    bool init() override;

    void update(float delta) override;
    void enableDebugDraw(const bool enable);
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_BOX2DCONTAINER_H */