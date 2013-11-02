#include <avalon/physics/loadercallbacks.h>
#include "Box2D/Box2D.h"
#include <avalon/physics/Box2dContainer.h>
#include <boost/any.hpp>

namespace avalon {
namespace physics {
namespace loadercallbacks {

avalon::io::TiledMapLoader::Callback createShapes(int filterCategory, bool isSensor)
{
    return [filterCategory](const avalon::io::TiledMapLoader::Configuration& config) {

        float x = boost::any_cast<float>(config.settings.at("x"));
        float y = boost::any_cast<float>(config.settings.at("y"));
        float w = boost::any_cast<float>(config.settings.at("width"));
        float h = boost::any_cast<float>(config.settings.at("height"));

        float density = 0.0;
        float friction = 1.0;
        float restitution = 0.0;
        std::string bodytype = "static";

        if (config.settings.count("friction")) {
            friction = boost::any_cast<float>(config.settings.at("friction"));
        }
        if (config.settings.count("density")) {
            density = boost::any_cast<float>(config.settings.at("density"));
        }
        if (config.settings.count("restitution")) {
            restitution = boost::any_cast<float>(config.settings.at("restitution"));
        }
        if (config.settings.count("bodytype")) {
            bodytype = boost::any_cast<std::string>(config.settings.at("bodytype"));
        }

        std::string type = "static";

        // create the body
        b2BodyDef bodyDef;

        if (type == "static") {
            bodyDef.type = b2_staticBody;
        } else if (type == "dynamic") {
            bodyDef.type = b2_dynamicBody;
        } else if (type == "kinematic") {
            bodyDef.type = b2_kinematicBody;
        } else {
            throw new std::invalid_argument("Unknown box2d type");
        }

        bodyDef.position.Set((x + (w / 2.0f)) / config.box2dContainer->pixelsInMeter,
                             (y + (h / 2.0f)) / config.box2dContainer->pixelsInMeter);
        b2Body* body = config.box2dContainer->world->CreateBody(&bodyDef);

        std::string shapeType = "rectangle";

        if (config.settings.count("polylinePoints") > 0)
            shapeType = "polylinePoints";

        if (config.settings.count("points") > 0)
            shapeType = "points";


        std::shared_ptr<b2Shape> shape;

        if (shapeType == "rectangle") {
            shape = initRectangleShape(w, h, config.box2dContainer->pixelsInMeter);
        } else if (shapeType == "polylinePoints") {
            auto pointList = boost::any_cast<std::list<cocos2d::Point>>(config.settings.at("polylinePoints"));
            if (pointList.size() == 2) {
                int c = 0;
                cocos2d::Point points[2];
                for (auto& p : pointList) {
                    points[c] = p;
                    ++c;
                }
                shape = initEdgeShape(points[0], points[1], config.box2dContainer->pixelsInMeter);
            } else {
                shape = initChainShape(pointList, config.box2dContainer->pixelsInMeter);
            }
        } else if (shapeType == "points") {
            auto pointList = boost::any_cast<std::list<cocos2d::Point>>(config.settings.at("points"));
            if (pointList.size() == 2) {
                int c = 0;
                cocos2d::Point points[2];
                for (auto& p : pointList) {
                    points[c] = p;
                    ++c;
                }
                shape = initEdgeShape(points[0], points[1], config.box2dContainer->pixelsInMeter);
            } else {
                shape = initChainShape(pointList, config.box2dContainer->pixelsInMeter, true);
            }
        }

        // create the fixture
        b2FixtureDef fixtureDef;
        fixtureDef.shape = shape.get();
        body->CreateFixture(&fixtureDef);
    };
}

std::shared_ptr<b2PolygonShape> initRectangleShape(float width, float height, float pixelsInMeter)
{
    auto shape = make_shared<b2PolygonShape>();
    shape->SetAsBox((width / pixelsInMeter) * 0.5f, (height / pixelsInMeter) * 0.5f);
    return shape;
}

std::shared_ptr<b2PolygonShape> initPolygonShape(int width, int height, float pixelsInMeter)
{
    auto shape = initRectangleShape(width, height, pixelsInMeter);
    return shape;
}

std::shared_ptr<b2CircleShape> initCircleShape()
{
    auto shape = make_shared<b2CircleShape>();
    return shape;
}

std::shared_ptr<b2ChainShape> initChainShape(std::list<cocos2d::Point> points, float pixelsInMeter, bool loop)
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
    
} // namesapce loadercallbacks
} // namespace physics
} // namespace avalon
