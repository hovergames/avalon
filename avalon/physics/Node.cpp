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

    if (body && box2dContainer) {
        loadFromBox2d();
    }
}

void Node::loadFromBox2d()
{
    auto size = getContentSize();
    auto pos = utils::convertFromBox2d(*box2dContainer, body->GetPosition(), size);

    // adjust to the current anchor point
    pos.x += getAnchorPoint().x * size.width;
    pos.y += getAnchorPoint().y * size.height;

    if (box2dContainer && getParent()) {
        pos = box2dContainer->convertToWorldSpace(pos);
        pos = getParent()->convertToNodeSpace(pos);
    }

    cocos2d::Node::setPosition({pos.x, pos.y});
    cocos2d::Node::setRotation(-CC_RADIANS_TO_DEGREES(body->GetAngle()));
}

bool Node::hasBody() const
{
    return body;
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
    loadFromBox2d();
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

void Node::setPosition(const cocos2d::Point& pos)
{
    cocos2d::Node::setPosition(pos);

    if (hasBody()) {
        using avalon::physics::utils::convertToBox2d;

        auto size = getContentSize();
        auto b2Pos = convertToBox2d(*box2dContainer, pos, size);

        // adjust to the current anchor point
        b2Pos.x -= getAnchorPoint().x * size.width / box2dContainer->pixelsInMeter;
        b2Pos.y -= getAnchorPoint().y * size.height / box2dContainer->pixelsInMeter;

        auto angle = getBody().GetTransform().q.GetAngle();
        getBody().SetTransform({b2Pos.x, b2Pos.y}, angle);
    }
}

void Node::setRotation(float rotation)
{
    cocos2d::Node::setRotation(rotation);

    if (hasBody()) {
        auto angle = CC_DEGREES_TO_RADIANS(rotation * -1);
        getBody().SetTransform(getBody().GetPosition(), angle);
    }
}


} // namespace physics
} // namespace avalon