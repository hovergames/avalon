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

b2Body* Box2dContainer::addBody(cocos2d::Node& node, const b2BodyDef& bodyDef)
{
    if (!nodeToId.count(&node)) {
        node.retain();
        auto newId = generateId();
        nodeToId[&node] = newId;
        idToNode[newId] = &node;
    }

    auto body = world->CreateBody(&bodyDef);
    body->SetUserData(static_cast<void*>(&nodeToId[&node]));
    return body;
}

template<typename T>
T* Box2dContainer::getNode(const b2Body& body)
{
    auto userDataPtr = body.GetUserData();
    if (!userDataPtr) {
        throw new std::invalid_argument("b2Body does not contain any user data");
    }

    auto nodeIdPtr = static_cast<NodeId*>(userDataPtr);
    auto iter = idToNode.find(*nodeIdPtr);
    if (iter == idToNode.end()) {
        throw new std::out_of_range("Unable to find node");
    }

    auto nodePtr = (*iter).second;
    auto resultPtr = dynamic_cast<T*>(nodePtr);
    if (!resultPtr) {
        throw new std::invalid_argument("Wrong node type");
    }

    return resultPtr;
}

Box2dContainer::NodeId Box2dContainer::generateId()
{
    return lastId++;
}

} // namespace physics
} // namespace avalon
