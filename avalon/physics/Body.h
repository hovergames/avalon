#ifndef AVALON_PHYSICS_BODY_H
#define AVALON_PHYSICS_BODY_H

#include "cocos2d.h"
#include <boost/noncopyable.hpp>

class b2Body;
namespace avalon { namespace physics { class Box2dContainer; } }

namespace avalon {
namespace physics {

class Body : boost::noncopyable
{
private:
    Box2dContainer& box2dContainer;
    b2Body& body;

    cocos2d::Point positionOffset;
    float rotationOffset = 0;
    cocos2d::Node* parent = nullptr;
    const cocos2d::Point anchorPoint{0.5, 0.5};
    cocos2d::Point position;
    cocos2d::Point nodeScale{1, 1};
    cocos2d::Point realScale{1, 1};
    float rotation = 0;

public:
    Body(avalon::physics::Box2dContainer& box2dContainer, b2Body& body);

    Box2dContainer& getBox2dContainer();
    b2Body& getBody();

    void sync(cocos2d::Node* parent);

    float getScaleX();
    float getScaleY();
    void setScale(float scale);
    void setScale(float scaleX, float scaleY);
    void setScaleX(float scaleX);
    void setScaleY(float scaleY);

    const cocos2d::Point& getPosition();
    void setPosition(const cocos2d::Point& pos);
    void setPosition(float x, float y);
    void setPositionX(float x);
    void setPositionY(float y);
    float getPositionX() const;
    float getPositionY() const;


    float getRotation();
    void setRotation(float rotation);
    void setRotationX(float rotationX);
    void setRotationY(float rotationY);

    const cocos2d::Point& getAnchorPoint() const;
    void setAnchorPoint(const cocos2d::Point& anchor);
    void ignoreAnchorPointForPosition(bool value);

    const cocos2d::Point& getPositionOffset();
    void setPositionOffset(const cocos2d::Point& point);

    float getRotationOffset();
    void setRotationOffset(float rotation);
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_BODY_H */