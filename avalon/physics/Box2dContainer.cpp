#include <avalon/physics/Box2dContainer.h>

#include <avalon/physics/vendors/B2DebugDrawLayer.h>
#include <avalon/physics/CollisionManager.h>

namespace avalon {
namespace physics {

bool Box2dContainer::init()
{
    if (!Node::init()) {
        return false;
    }

    world.reset(new b2World({0, 0}));
    scheduleUpdate();

    return true;
}

std::shared_ptr<CollisionManager> Box2dContainer::getCollisionManager()
{
    if (!collisionManager) {
        collisionManager.reset(new CollisionManager(*this));
        world->SetContactListener(getCollisionManager().get());
    }

    return collisionManager;
}

void Box2dContainer::update(float delta)
{
    Node::update(delta);

    executePendingDeletes();
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
        auto newId = generateId();
        nodeToId[&node] = newId;
        idToNode[newId] = &node;
    }

    auto body = world->CreateBody(&bodyDef);
    body->SetUserData(static_cast<void*>(nodeToId[&node]));
    return body;
}

void Box2dContainer::removeNode(cocos2d::Node& node)
{
    auto iter = nodeToId.find(&node);
    if (iter == nodeToId.end()) {
        return;
    }

    nodeToId.erase(iter);
    idToNode.erase((*iter).second);
}

void Box2dContainer::executePendingDeletes()
{
    for (auto& pair : pendingDeletes) {
        world->DestroyBody(pair.first);
        if (pair.second) {
            removeNode(*pair.second);
        }
    }

    pendingDeletes.clear();
}

void Box2dContainer::destroyBodyDelayed(b2Body& body)
{
    pendingDeletes.push_back(std::make_pair(&body, nullptr));
}

void Box2dContainer::destroyBodyDelayed(b2Body& body, cocos2d::Node& node)
{
    pendingDeletes.push_back(std::make_pair(&body, &node));
}

Box2dContainer::NodeId Box2dContainer::generateId()
{
    return ++lastNodeId;
}

} // namespace physics
} // namespace avalon
