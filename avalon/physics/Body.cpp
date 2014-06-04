#include <avalon/physics/Body.h>

#include <avalon/physics/Box2dContainer.h>

namespace avalon {
namespace physics {

Body::Body(avalon::physics::Box2dContainer& box2dContainer, b2Body& body)
: box2dContainer(box2dContainer)
, body(body)
{
}

Box2dContainer& Body::getBox2dContainer()
{
    return box2dContainer;
}

b2Body& Body::getBody()
{
    return body;
}

void Body::clearFixtures()
{
    for (auto f = getBody().GetFixtureList(); f; f = f->GetNext()) {
        getBody().DestroyFixture(f);
    }
}

void Body::sync(cocos2d::Node* node)
{
    parent = node->getParent();

    // -- Base Transform
    auto transform = box2dContainer.getNodeToWorldTransform();
    auto b_a = transform.m[0];
    auto b_b = transform.m[1];
    auto b_c = transform.m[4];
    auto b_d = transform.m[5];

    auto baseScaleX = sqrt(b_a * b_a + b_c * b_c);
    auto baseScaleY = sqrt(b_b * b_b + b_d * b_d);
    auto baseRotation = atan2(b_b, b_a);

    if (baseScaleX == 0) baseScaleX = 1;
    if (baseScaleY == 0) baseScaleY = 1;

    // -- Optional parent transform
    float parentScaleX = 0;
    float parentScaleY = 0;
    float parentRotation = 0;

    if (parent) {
        auto transform = parent->getNodeToWorldTransform();
        auto p_a = transform.m[0];
        auto p_b = transform.m[1];
        auto p_c = transform.m[4];
        auto p_d = transform.m[5];

        parentScaleX = sqrt(p_a * p_a + p_c * p_c);
        parentScaleY = sqrt(p_b * p_b + p_d * p_d);
        parentRotation = atan2(p_b, p_a);

        if (parentScaleX == 0) parentScaleX = 1;
        if (parentScaleY == 0) parentScaleY = 1;
    }

    // -- Position offset
    auto positionB2d = box2dContainer.convertToBox2d(positionOffset);
    b2Rot rotationB2d(body.GetAngle());
    positionB2d = b2Mul(rotationB2d, positionB2d) + body.GetPosition();

    // -- Position
    position = box2dContainer.convertFromBox2d(positionB2d);
    position = box2dContainer.convertToWorldSpace(position);
    if (parent) {
        position = parent->convertToNodeSpace(position);
    }

    // -- Scaling
    auto parentScaleXAbs = parentScaleX / baseScaleX;
    auto parentScaleYAbs = parentScaleY / baseScaleY;
    auto parentRotationAbs = parentRotation - baseRotation;

    if (parentScaleXAbs == 0) parentScaleXAbs = 1;
    if (parentScaleYAbs == 0) parentScaleYAbs = 1;

    realScale.x = nodeScale.x / parentScaleXAbs;
    realScale.y = nodeScale.y / parentScaleYAbs;

    // -- Rotation
    rotation = body.GetAngle() - parentRotationAbs;
    rotation = rotationOffset - CC_RADIANS_TO_DEGREES(rotation);
}

const cocos2d::Point& Body::getAnchorPoint() const
{
    return anchorPoint;
}

const cocos2d::Point& Body::getPosition()
{
    return position;
}

float Body::getRotation()
{
    return rotation;
}

float Body::getScaleX()
{
    return realScale.x;
}

float Body::getScaleY()
{
    return realScale.y;
}

void Body::setScale(float scale)
{
    nodeScale.x = scale;
    nodeScale.y = scale;
};

void Body::setScale(float scaleX, float scaleY)
{
    nodeScale.x = scaleX;
    nodeScale.y = scaleY;
};

void Body::setScaleX(float scaleX)
{
    nodeScale.x = scaleX;
};

void Body::setScaleY(float scaleY)
{
    nodeScale.y = scaleY;
};

void Body::setPosition(const cocos2d::Point& pos)
{
    setPosition(pos.x, pos.y);
};

void Body::setPosition(float x, float y)
{
    auto pos = getBox2dContainer().convertToBox2d({x, y});
    getBody().SetTransform(pos, getBody().GetAngle());
};

void Body::setPositionX(float x)
{
    setPosition(x, getPositionY());
}

void Body::setPositionY(float y)
{
    setPosition(getPositionX(), y);
}

float Body::getPositionX() const
{
    return position.x;
}

float Body::getPositionY() const
{
    return position.y;
}

void Body::setRotation(float rotation)
{
    getBody().SetTransform(getBody().GetPosition(), CC_DEGREES_TO_RADIANS(rotation));
};

void Body::setRotationX(float rotationX)
{
    throw std::invalid_argument("Not implemented yet!");
};

void Body::setRotationY(float rotationY)
{
    throw std::invalid_argument("Not implemented yet!");
};

void Body::setAnchorPoint(const cocos2d::Point& anchor)
{
    throw std::invalid_argument("Not allowed!");
};

void Body::ignoreAnchorPointForPosition(bool value)
{
    throw std::invalid_argument("Not allowed!");
};

void Body::setPositionOffset(const cocos2d::Point& point)
{
    positionOffset = point;
}

const cocos2d::Point& Body::getPositionOffset()
{
    return positionOffset;
}

void Body::setRotationOffset(float rotation)
{
    rotationOffset = rotation;
}

float Body::getRotationOffset()
{
    return rotationOffset;
}

} // namespace physics
} // namespace avalon