#include <avalon/physics/Box2dContainer.h>

#include <avalon/physics/vendors/B2DebugDrawLayer.h>

namespace avalon {
namespace physics {

Box2dContainer::~Box2dContainer()
{
    for (auto& pair : nodeToId) {
        pair.first->release();
    }
    nodeToId.clear();
    idToNode.clear();
}

bool Box2dContainer::init()
{
    if (!Node::init()) {
        return false;
    }

    world.reset(new b2World(b2Vec2(0, 0)));
    scheduleUpdate();

    return true;
}

void Box2dContainer::update(float delta)
{
    Node::update(delta);
    world->Step(timeStep, velocityIterations, positionIterations);
}

void Box2dContainer::enableDebugDraw(const bool enable)
{
    if (!enable && !debugLayer) {
        return;
    }

    if (!debugLayer) {
        debugLayer = B2DebugDrawLayer::create(world.get(), pixelsInMeter);
        addChild(debugLayer, std::numeric_limits<int>::max());
    }

    debugLayer->setVisible(enable);
}

b2Body* Box2dContainer::createBody(const b2BodyDef& bodyDef)
{
    return world->CreateBody(&bodyDef);
}

b2Body* Box2dContainer::createBody(const b2BodyDef& bodyDef, cocos2d::Node& node)
{
    if (!nodeToId.count(&node)) {
        node.retain();
        auto newId = generateId();
        nodeToId[&node] = newId;
        idToNode[newId] = &node;
    }

    auto body = world->CreateBody(&bodyDef);
    body->SetUserData(static_cast<void*>(nodeToId[&node]));
    return body;
}

Box2dContainer::NodeId Box2dContainer::generateId()
{
    NodeId result = nullptr;
    do {
        if (result) {
            delete result;
        }
        result = new int(0);
    } while (idToNode.count(result));
    delete result;
    return result;
}

} // namespace physics
} // namespace avalon
