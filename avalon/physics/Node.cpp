#include <avalon/physics/Node.h>

#include <avalon/physics/Box2dContainer.h>

namespace avalon {
namespace physics {

bool Node::init(avalon::physics::Box2dContainer& box2dContainer, b2Body& body)
{
    if (!cocos2d::Node::init()) {
        return false;
    }

    bodyImpl.reset(new Body(box2dContainer, body));
    getBox2dContainer().assignNode(getBody(), *this);

    scheduleUpdate();

    return true;
}

void Node::cleanup()
{
    bodyImpl->getBox2dContainer().removeNode(*this);
    bodyImpl->getBox2dContainer().getWorld().DestroyBody(&bodyImpl->getBody());

    cocos2d::Node::cleanup();
}

void Node::update(float delta)
{
    bodyImpl->sync(this);
    cocos2d::Node::setAnchorPoint(bodyImpl->getAnchorPoint());
    cocos2d::Node::setPosition(bodyImpl->getPosition());
    cocos2d::Node::setRotation(bodyImpl->getRotation());
    cocos2d::Node::setScaleX(bodyImpl->getScaleX());
    cocos2d::Node::setScaleY(bodyImpl->getScaleY());

    cocos2d::Node::update(delta);
}

} // namespace physics
} // namespace avalon