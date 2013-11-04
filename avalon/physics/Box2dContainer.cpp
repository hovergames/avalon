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

CollisionManager& Box2dContainer::getCollisionManager()
{
    if (!collisionManager) {
        collisionManager.reset(new CollisionManager(*this));
        world->SetContactListener(collisionManager.get());
    }

    return *collisionManager.get();
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
    auto body = world->CreateBody(&bodyDef);
    assignNode(*body, node);
    return body;
}

void Box2dContainer::assignNode(b2Body& body, cocos2d::Node& node)
{
    if (!nodeToId.count(&node)) {
        auto newId = generateId();
        nodeToId[&node] = newId;
        idToNode[newId] = &node;
    }

    body.SetUserData(static_cast<void*>(nodeToId[&node]));
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

b2World& Box2dContainer::getWorld()
{
    return *world.get();
}

void Box2dContainer::executePendingDeletes()
{
    for (auto& pair : pendingDeletes) {
        auto body = pair.first;
        auto node = pair.second;

        if (body) {
            world->DestroyBody(body);
        }
        if (body && node) {
            removeNode(*node);
            node->release();
        }
        if (!body && node) {
            node->removeFromParent();
            node->release();
        }
    }

    pendingDeletes.clear();
}

void Box2dContainer::destroyDelayed(b2Body& body)
{
    pendingDeletes.push_back(std::make_pair(&body, nullptr));
}

void Box2dContainer::destroyDelayed(b2Body& body, cocos2d::Node& node)
{
    node.retain();
    pendingDeletes.push_back(std::make_pair(&body, &node));
}

void Box2dContainer::destroyDelayed(cocos2d::Node& node)
{
    node.retain();
    pendingDeletes.push_back(std::make_pair(nullptr, &node));
}

Box2dContainer::NodeId Box2dContainer::generateId()
{
    return ++lastNodeId;
}

} // namespace physics
} // namespace avalon
