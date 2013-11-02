#include <avalon/physics/loadercallbacks.h>
#include "Box2D/Box2D.h"
#include <avalon/physics/Box2dContainer.h>
#include <boost/any.hpp>

namespace {

std::shared_ptr<b2PolygonShape> initRectangleShape(float width, float height, float pixelsInMeter)
{
    auto shape = make_shared<b2PolygonShape>();
    shape->SetAsBox((width / pixelsInMeter) * 0.5f, (height / pixelsInMeter) * 0.5f);
    return shape;
}

std::shared_ptr<b2ChainShape> initChainShape(std::list<cocos2d::Point> points, float pixelsInMeter, bool loop = false)
{
    std::vector<b2Vec2> vecs;
    vecs.reserve(points.size());

    for (auto& p : points) {
        p = p / pixelsInMeter;
        vecs.push_back({p.x, -p.y});
    }

    auto shape = make_shared<b2ChainShape>();

    if (loop) {
        shape->CreateLoop(&vecs[0], points.size());
    } else {
        shape->CreateChain(&vecs[0], points.size());
    }

    return shape;
}

std::shared_ptr<b2EdgeShape> initEdgeShape(cocos2d::Point p1, cocos2d::Point p2, float pixelsInMeter)
{
    p1 = p1 / pixelsInMeter;
    p2 = p2 / pixelsInMeter;
    auto shape = make_shared<b2EdgeShape>();
    shape->Set({p1.x, p1.y}, {p2.x, p2.y});
    return shape;
}

std::shared_ptr<b2Shape> initShapeFromPoints(const std::list<cocos2d::Point>& points, float pixelsInMeter, bool loop = false)
{
    if (points.size() == 2) {
        return initEdgeShape(points.front(), points.back(), pixelsInMeter);
    } else {
        return initChainShape(points, pixelsInMeter, loop);
    }
}

} // namespace

namespace avalon {
namespace physics {
namespace loadercallbacks {

avalon::io::TiledMapLoader::Callback createShapes(int filterCategory, bool isSensor)
{
    return [filterCategory, isSensor](const avalon::io::TiledMapLoader::Configuration& config) {

        float x = boost::any_cast<float>(config.settings.at("x"));
        float y = boost::any_cast<float>(config.settings.at("y"));
        float width = boost::any_cast<float>(config.settings.at("width"));
        float height = boost::any_cast<float>(config.settings.at("height"));
        float pixelsInMeter = config.box2dContainer->pixelsInMeter;

        float density = 0.0;
        float friction = 1.0;
        float restitution = 0.0;
        std::string bodytype = "static";

        if (config.settings.count("friction"))      friction = boost::any_cast<float>(config.settings.at("friction"));
        if (config.settings.count("density"))       density = boost::any_cast<float>(config.settings.at("density"));
        if (config.settings.count("restitution"))   restitution = boost::any_cast<float>(config.settings.at("restitution"));
        if (config.settings.count("bodytype"))      bodytype = boost::any_cast<std::string>(config.settings.at("bodytype"));

        // create the body
        b2BodyDef bodyDef;

        if (bodytype == "static")           bodyDef.type = b2_staticBody;
        else if (bodytype == "dynamic")     bodyDef.type = b2_dynamicBody;
        else if (bodytype == "kinematic")   bodyDef.type = b2_kinematicBody;
        else                                throw new std::invalid_argument("Unknown box2d type");

        bodyDef.position.Set((x + (width / 2.0f)) / pixelsInMeter, (y + (height / 2.0f)) / pixelsInMeter);
        b2Body* body = config.box2dContainer->world->CreateBody(&bodyDef);

        std::shared_ptr<b2Shape> shape;

        if (config.settings.count("polylinePoints")) {
            auto points = boost::any_cast<std::list<cocos2d::Point>>(config.settings.at("polylinePoints"));
            shape = initShapeFromPoints(points, pixelsInMeter);
        } else if (config.settings.count("points") > 0) {
            auto points = boost::any_cast<std::list<cocos2d::Point>>(config.settings.at("points"));
            shape = initShapeFromPoints(points, pixelsInMeter, true);
        } else {
            shape = initRectangleShape(width, height, pixelsInMeter);
        }

        b2FixtureDef fixtureDef;
        fixtureDef.shape = shape.get();
        fixtureDef.isSensor = isSensor;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.density = density;
        fixtureDef.filter.categoryBits = filterCategory;
        fixtureDef.filter.maskBits = 0xFFFF;
        fixtureDef.filter.groupIndex = 0;
        
        body->CreateFixture(&fixtureDef);
    };
}

} // namespace loadercallbacks
} // namespace physics
} // namespace avalon
