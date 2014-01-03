#include <avalon/physics/utils.h>

#include <boost/any.hpp>
#include <avalon/physics/Box2dContainer.h>

namespace {

std::shared_ptr<b2PolygonShape> initRectangleShape(float width, float height, float pixelsInMeter)
{
    auto shape = std::make_shared<b2PolygonShape>();
    shape->SetAsBox((width / pixelsInMeter) * 0.5f, (height / pixelsInMeter) * 0.5f);
    return shape;
}

std::shared_ptr<b2ChainShape> initChainShape(const cocos2d::ValueVector& points, float pixelsInMeter, bool loop = false)
{
    auto convert = [&pixelsInMeter](const cocos2d::Value& value) -> b2Vec2 {
        return {value.asValueMap().at("x").asFloat() / pixelsInMeter, (value.asValueMap().at("y").asFloat() / pixelsInMeter) * -1};
    };

    std::vector<b2Vec2> vecs;
    vecs.reserve(points.size());
    std::transform(points.begin(), points.end(), vecs.begin(), convert);

    auto shape = std::make_shared<b2ChainShape>();
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
    auto shape = std::make_shared<b2EdgeShape>();
    shape->Set({p1.x, p1.y}, {p2.x, p2.y});
    return shape;
}

std::shared_ptr<b2Shape> initShapeFromPoints(const cocos2d::ValueVector& points, float pixelsInMeter, bool loop = false)
{
    if (points.size() == 2) {
        cocos2d::Point p1 {points.front().asValueMap().at("x").asFloat(), points.front().asValueMap().at("y").asFloat()};
        cocos2d::Point p2 {points.back().asValueMap().at("x").asFloat(), points.back().asValueMap().at("y").asFloat()};
        return initEdgeShape(p1, p2, pixelsInMeter);
    } else {
        return initChainShape(points, pixelsInMeter, loop);
    }
}

} // namespace

namespace avalon {
namespace physics {
namespace utils {

b2BodyType getBodyTypeFromString(const std::string& type)
{
    if      (type == "static")    return b2_staticBody;
    else if (type == "dynamic")   return b2_dynamicBody;
    else if (type == "kinematic") return b2_kinematicBody;
    else                          throw new std::invalid_argument("Unknown box2d type");
}

avalon::io::TiledMapLoader::Callback shapeLoader(int filterCategory, bool isSensor)
{
    return [filterCategory, isSensor](const avalon::io::TiledMapLoader::Configuration& config)
    {
        const float x = config.settings.at("x").asFloat();
        const float y = config.settings.at("y").asFloat();
        const float width = config.settings.at("width").asFloat();
        const float height = config.settings.at("height").asFloat();
        const float pixelsInMeter = config.box2dContainer->pixelsInMeter;
        const auto pos = config.box2dContainer->convertToBox2d({x + width * 0.5, y + height * 0.5});

        auto fixtureDef = config.box2dContainer->defaultFixtureDef;
        fixtureDef.isSensor = isSensor;
        fixtureDef.filter.categoryBits = filterCategory;

        std::string bodytype = "static";
        if (config.settings.count("friction"))    fixtureDef.friction = config.settings.at("friction").asFloat();
        if (config.settings.count("density"))     fixtureDef.density = config.settings.at("density").asFloat();
        if (config.settings.count("restitution")) fixtureDef.restitution = config.settings.at("restitution").asFloat();
        if (config.settings.count("bodytype"))    bodytype = config.settings.at("bodytype").asString();

        std::shared_ptr<b2Shape> shape;
        if (config.settings.count("polylinePoints")) {
            auto points = config.settings.at("polylinePoints").asValueVector();
            shape = initShapeFromPoints(points, pixelsInMeter);
        } else if (config.settings.count("points") > 0) {
            auto points = config.settings.at("points").asValueVector();
            shape = initShapeFromPoints(points, pixelsInMeter, true);
        } else {
            cocos2d::log("%f,%f,%f,%f",x,y,width,height);
            shape = initRectangleShape(width, height, pixelsInMeter);
        }
        fixtureDef.shape = shape.get();

        b2BodyDef bodyDef;
        bodyDef.type = getBodyTypeFromString(bodytype);
        bodyDef.position.Set(pos.x, pos.y);

        auto body = config.box2dContainer->getWorld().CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);
    };
}

} // namespace utils
} // namespace physics
} // namespace avalon
