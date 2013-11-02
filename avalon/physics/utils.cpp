#include <avalon/physics/utils.h>

#include <boost/any.hpp>
#include <avalon/physics/Box2dContainer.h>

namespace {

std::shared_ptr<b2PolygonShape> initRectangleShape(float width, float height, float pixelsInMeter)
{
    auto shape = make_shared<b2PolygonShape>();
    shape->SetAsBox((width / pixelsInMeter) * 0.5f, (height / pixelsInMeter) * 0.5f);
    return shape;
}

std::shared_ptr<b2ChainShape> initChainShape(std::list<cocos2d::Point> points, float pixelsInMeter, bool loop = false)
{
    auto convert = [&pixelsInMeter](const cocos2d::Point& p) -> b2Vec2 {
        return {p.x / pixelsInMeter, (p.y / pixelsInMeter) * -1};
    };

    std::vector<b2Vec2> vecs;
    vecs.reserve(points.size());
    std::transform(points.begin(), points.end(), vecs.begin(), convert);

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
namespace utils {

b2Vec2 convertToBox2d(const avalon::physics::Box2dContainer& box2dContainer, const cocos2d::Point& pos, const cocos2d::Size& size)
{
    b2Vec2 result(pos.x, pos.y);

    // box2d handle is in the center
    result.x += size.width / 2;
    result.y += size.height / 2;

    // convert from cocos2d to box2d units
    result.x /= box2dContainer.pixelsInMeter;
    result.y /= box2dContainer.pixelsInMeter;

    return result;
}

cocos2d::Point convertFromBox2d(const avalon::physics::Box2dContainer& box2dContainer, const b2Vec2& pos, const cocos2d::Size& size)
{
    cocos2d::Point result(pos.x, pos.y);

    // convert from box2d to cocos2d units
    result.x *= box2dContainer.pixelsInMeter;
    result.y *= box2dContainer.pixelsInMeter;

    // cocos2dx handle is in the lower left corner
    result.x -= size.width / 2;
    result.y -= size.height / 2;

    return result;
}

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
        const float x = boost::any_cast<float>(config.settings.at("x"));
        const float y = boost::any_cast<float>(config.settings.at("y"));
        const float width = boost::any_cast<float>(config.settings.at("width"));
        const float height = boost::any_cast<float>(config.settings.at("height"));
        const float pixelsInMeter = config.box2dContainer->pixelsInMeter;
        const auto pos = convertToBox2d(*config.box2dContainer, {x, y}, {width, height});

        auto fixtureDef = config.box2dContainer->defaultFixtureDef;
        fixtureDef.isSensor = isSensor;
        fixtureDef.filter.categoryBits = filterCategory;

        std::string bodytype = "static";
        if (config.settings.count("friction"))    fixtureDef.friction = boost::any_cast<float>(config.settings.at("friction"));
        if (config.settings.count("density"))     fixtureDef.density = boost::any_cast<float>(config.settings.at("density"));
        if (config.settings.count("restitution")) fixtureDef.restitution = boost::any_cast<float>(config.settings.at("restitution"));
        if (config.settings.count("bodytype"))    bodytype = boost::any_cast<std::string>(config.settings.at("bodytype"));

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
        fixtureDef.shape = shape.get();

        b2BodyDef bodyDef;
        bodyDef.type = getBodyTypeFromString(bodytype);
        bodyDef.position.Set(pos.x, pos.y);

        auto body = config.box2dContainer->world->CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);
    };
}

} // namespace utils
} // namespace physics
} // namespace avalon
