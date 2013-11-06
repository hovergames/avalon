#include <avalon/physics/Node.h>

#include <avalon/physics/utils.h>
#include <avalon/physics/Box2dContainer.h>

namespace avalon {
namespace physics {

bool Node::init()
{
    if (!cocos2d::Node::init()) {
        return false;
    }

    scheduleUpdate();

    return true;
}

void Node::cleanup()
{
    removeOldBody();
    cocos2d::Node::cleanup();
}

void Node::update(float delta)
{
    cocos2d::Node::update(delta);

    if (!body || !box2dContainer) {
        return;
    }

    auto size = getContentSize();
    auto pos = utils::convertFromBox2d(*box2dContainer, body->GetPosition(), size);

    // adjust to the current anchor point
    pos.x += getAnchorPoint().x * size.width;
    pos.y += getAnchorPoint().y * size.height;
    pos.x *= getParent()->getScaleX();
    pos.y *= getParent()->getScaleY();

    cocos2d::Node::setPosition({pos.x, pos.y});
    cocos2d::Node::setRotation(-CC_RADIANS_TO_DEGREES(body->GetAngle()));
}

b2Body& Node::getBody()
{
    return *body;
}

void Node::setBody(b2Body& body)
{
    removeOldBody();

    this->body = &body;
    getBox2dContainer().assignNode(*this->body, *this);
}

void Node::removeOldBody()
{
    if (!body) {
        return;
    }

    getBox2dContainer().removeNode(*this);
    getBox2dContainer().getWorld().DestroyBody(body);
    body = nullptr;
}

Box2dContainer& Node::getBox2dContainer()
{
    return *box2dContainer;
}

void Node::setBox2dContainer(Box2dContainer& box2dContainer)
{
    this->box2dContainer = &box2dContainer;
}

} // namespace physics
} // namespace avalon