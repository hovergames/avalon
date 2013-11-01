#include <avalon/physics/Sprite.h>

#include <avalon/utils/tiled.h>
#include <avalon/physics/Box2dContainer.h>
#include <avalon/physics/vendors/GB2ShapeCache.h>

namespace avalon {
namespace physics {

Sprite::~Sprite()
{
    if (hasBody()) {
        getBody().GetWorld()->DestroyBody(&getBody());
    }
    if (box2dContainer) {
        box2dContainer->removeNode(*this);
    }
}

bool Sprite::hasBody() const
{
    return !!body;
}

b2Body& Sprite::getBody()
{
    if (!body) {
        throw new std::runtime_error("no body created yet");
    }

    return *body;
}

void Sprite::onConfiguration(const avalon::io::CCBLoader::Configuration& config)
{
    if (config.settings.count("pes.file")) {
        if (!config.settings.count("pes.shape")) {
            throw std::invalid_argument("pes.shape not defined!");
        }

        auto file = boost::any_cast<std::string>(config.settings.at("pes.file"));
        auto shape = boost::any_cast<std::string>(config.settings.at("pes.shape"));
        addFixtureToBodyFromPESFile(file, shape, *config.box2dContainer);
    } else {
        createBody(*config.box2dContainer);
        createDefaultShape();
    }

    float x = getPositionX();
    float y = getPositionY();
    setPosition({x, y});

    if (config.settings.count("friction")) {
        getBody().GetFixtureList()->SetFriction(boost::any_cast<float>(config.settings.at("friction")));
    }
    if (config.settings.count("density")) {
        getBody().GetFixtureList()->SetDensity(boost::any_cast<float>(config.settings.at("density")));
    }
    if (config.settings.count("restitution")) {
        getBody().GetFixtureList()->SetRestitution(boost::any_cast<float>(config.settings.at("restitution")));
    }
    if (config.settings.count("bodytype")) {
        getBody().SetType(getBox2dBodyType(boost::any_cast<std::string>(config.settings.at("bodytype"))));
    }
}

void Sprite::onConfiguration(const avalon::io::TiledMapLoader::Configuration& config)
{
    if (config.settings.count("pes.file")) {
        if (!config.settings.count("pes.shape")) {
            throw std::invalid_argument("pes.shape not defined!");
        }

        auto file = config.settings.at("pes.file");
        auto shape = config.settings.at("pes.shape");
        addFixtureToBodyFromPESFile(file, shape, *config.box2dContainer);
    } else {
        createBody(*config.box2dContainer);
        createDefaultShape();
    }

    float x = std::stoi(config.settings.at("x"));
    float y = std::stoi(config.settings.at("y"));
    auto pos = avalon::utils::tiled::getPositionFromCoord(config.map, x, y);
    setPosition(pos);

    if (config.settings.count("friction")) {
        getBody().GetFixtureList()->SetFriction(std::stof(config.settings.at("friction")));
    }
    if (config.settings.count("density")) {
        getBody().GetFixtureList()->SetDensity(std::stof(config.settings.at("density")));
    }
    if (config.settings.count("restitution")) {
        getBody().GetFixtureList()->SetRestitution(std::stof(config.settings.at("restitution")));
    }
    if (config.settings.count("bodytype")) {
        getBody().SetType(getBox2dBodyType(config.settings.at("bodytype")));
    }
}

b2BodyType Sprite::getBox2dBodyType(const std::string& type)
{
    if (type == "static") {
        return b2_staticBody;
    } else if (type == "dynamic") {
        return b2_dynamicBody;
    } else if (type == "kinematic") {
        return b2_kinematicBody;
    } else {
        throw new std::invalid_argument("Unknown box2d type");
    }
}

void Sprite::createDefaultShape()
{
    float w = getContentSize().width;
    float h = getContentSize().height;

    w = (w / box2dContainer->pixelsInMeter) * 0.5f;
    h = (h / box2dContainer->pixelsInMeter) * 0.5f;

    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    getBody().CreateFixture(&fixtureDef);
}

void Sprite::createBody(Box2dContainer& box2dContainer)
{
    this->box2dContainer = &box2dContainer;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    body = box2dContainer.createBody(bodyDef, *this);
}

void Sprite::addFixtureToBodyFromPESFile(const std::string& file, const std::string& shape, Box2dContainer& box2DContainer)
{
    if (!hasBody()) {
        createBody(box2DContainer);
    }

    auto cache = cocos2d::GB2ShapeCache::sharedGB2ShapeCache();
    cache->addShapesWithFile(file);
    cache->addFixturesToBody(&getBody(), shape);

    setAnchorPoint(cache->anchorPointForShape(shape));
}

bool Sprite::init()
{
    if (!cocos2d::Sprite::init()) {
        return false;
    }
    
    scheduleUpdate();
    return true;
}

void Sprite::update(float delta)
{
    cocos2d::Sprite::update(delta);

    if (hasBody()) {
        auto size = getContentSize();
        auto x = body->GetPosition().x;
        auto y = body->GetPosition().y;

        // convert from box2d to cocos2dx units
        x *= box2dContainer->pixelsInMeter;
        y *= box2dContainer->pixelsInMeter;

        // cocos2dx handle is in the lower left corner
        x -= size.width / 2;
        y -= size.height / 2;

        // adjust to the current anchor point
        x += getAnchorPoint().x * size.width;
        y += getAnchorPoint().y * size.height;

        cocos2d::Sprite::setPosition({x, y});

        float rotation = body->GetAngle();
        cocos2d::Sprite::setRotation(-CC_RADIANS_TO_DEGREES(rotation));
    }
}

void Sprite::setPosition(const cocos2d::Point& pos)
{
    cocos2d::Sprite::setPosition(pos);

    if (hasBody()) {
        auto size = getContentSize();
        auto x = pos.x;
        auto y = pos.y;

        // box2d handle is in the center
        x += size.width * 0.5f;
        y += size.height * 0.5f;

        // adjust to the current anchor point
        x -= getAnchorPoint().x * size.width;
        y -= getAnchorPoint().y * size.height;

        // convert from cocos2d to box2d units
        x /= box2dContainer->pixelsInMeter;
        y /= box2dContainer->pixelsInMeter;

        auto angle = getBody().GetTransform().q.GetAngle();
        getBody().SetTransform({x, y}, angle);
    }
}

} // namespace physics
} // namespace avalon
