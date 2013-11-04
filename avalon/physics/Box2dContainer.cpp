#include <avalon/physics/Box2dContainer.h>

#include <avalon/physics/vendors/b2dJson/b2dJson.h>
#include <avalon/physics/vendors/b2dJson/b2dJsonImage.h>
#include <avalon/physics/vendors/B2DebugDrawLayer.h>
#include <avalon/physics/CollisionManager.h>
#include <avalon/physics/Sprite.h>
#include <avalon/io/utils.h>

namespace avalon {
namespace physics {

Box2dContainer* Box2dContainer::createFromJson(const std::string& jsonFile)
{
    auto container = new Box2dContainer();
    if (container && container->initFromJson(jsonFile)) {
        container->autorelease();
        return container;
    } else {
        delete container;
        return nullptr;
    }
}

bool Box2dContainer::initFromJson(const std::string& jsonFile)
{
    using avalon::io::utils::getStringFromFile;

    if (!Node::init()) {
        return false;
    }

    std::string error;
    json.reset(new b2dJson());
    world.reset(json->readFromString(getStringFromFile(jsonFile), error));
    if (error.length() > 0) {
        throw new std::runtime_error(error);
    }

    loadAllImagesFromJson();
    scheduleUpdate();

    return true;
}

void Box2dContainer::loadAllImagesFromJson()
{
    std::vector<b2dJsonImage*> b2dImages;
    json->getAllImages(b2dImages);

    cocos2d::Sprite* sprite = nullptr;
    for (auto& image : b2dImages) {
        if (image->body) {
            auto physicSprite = avalon::physics::Sprite::create(*this, *image->body, image->file.c_str());
            physicSprite->setCenter({image->center.x, image->center.y});
            sprite = physicSprite;
        } else {
            sprite = cocos2d::Sprite::create(image->file.c_str());
            sprite->setPosition({image->center.x, image->center.y});
        }

        sprite->setFlippedX(image->flip);
        sprite->setColor({image->colorTint[0], image->colorTint[1], image->colorTint[2]});
        sprite->setOpacity(image->colorTint[3]);
        sprite->setScale(image->scale);
        sprite->setRotation(CC_RADIANS_TO_DEGREES(-image->angle));
        addChild(sprite, image->renderOrder);
    }
}

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

b2dJson& Box2dContainer::getJson()
{
    if (!json) {
        throw new std::runtime_error("World not loaded from a json file");
    }
    return *json.get();
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

namespace std {

std::string to_string(avalon::physics::Box2dContainer& box2dContainer)
{
    b2dJson json;
    return json.writeToString(&box2dContainer.getWorld());
}

} // namespace std
