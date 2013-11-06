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

    cocos2d::Node::setAnchorPoint({0.5, 0.5});
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
    
    auto pos = box2dContainer->convertFromBox2d(body->GetPosition());
    pos = box2dContainer->convertToWorldSpace(pos);
    if (getParent()) pos = getParent()->convertToNodeSpace(pos);
    cocos2d::Node::setPosition({pos.x, pos.y});

    auto b = box2dContainer->getNodeToWorldTransform();
    auto bScaleX = sqrt(b.a * b.a + b.c * b.c);
    auto bScaleY = sqrt(b.b * b.b + b.d * b.d);
    auto bRotation = atan2(b.b, b.a);

    auto p = getParent()->getNodeToWorldTransform();
    auto pScaleX = sqrt(p.a * p.a + p.c * p.c);
    auto pScaleY = sqrt(p.b * p.b + p.d * p.d);
    auto pRotation = atan2(p.b, p.a);

    if (bScaleX == 0) bScaleX = 1;
    if (bScaleY == 0) bScaleY = 1;

    auto pScaleXAbs = pScaleX / bScaleX;
    auto pScaleYAbs = pScaleY / bScaleY;
    auto pRotationAbs = pRotation - bRotation;

    if (pScaleXAbs == 0) pScaleXAbs = 1;
    if (pScaleYAbs == 0) pScaleYAbs = 1;

    cocos2d::Node::setScaleX(myScaleX / pScaleXAbs);
    cocos2d::Node::setScaleY(myScaleY / pScaleYAbs);

    float rotation = body->GetAngle() - pRotationAbs;
    cocos2d::Node::setRotation(-CC_RADIANS_TO_DEGREES(rotation));
}

bool Node::hasBody() const
{
    return body;
}

b2Body& Node::getBody()
{
    if (!body) {
        throw new std::runtime_error("not attached to any body yet");
    }

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
    if (!box2dContainer) {
        throw new std::runtime_error("not attached to any box2dContainer yet");
    }

    return *box2dContainer;
}

void Node::setBox2dContainer(Box2dContainer& box2dContainer)
{
    this->box2dContainer = &box2dContainer;
}

} // namespace physics
} // namespace avalon