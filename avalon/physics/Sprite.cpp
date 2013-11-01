#include <avalon/physics/Sprite.h>

#include <avalon/utils/tiled.h>
#include <avalon/physics/Box2dContainer.h>
#include <avalon/physics/vendors/GB2ShapeCache.h>

namespace avalon {
namespace physics {

Sprite* Sprite::create(avalon::physics::Box2dContainer& box2dContainer, const char *filename)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->createBody();
    sprite->addTextureShape();
    return sprite;
}

Sprite* Sprite::create(avalon::physics::Box2dContainer& box2dContainer, const char *filename, const cocos2d::Rect& rect)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename, rect)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->createBody();
    sprite->addTextureShape();
    return sprite;
}

Sprite* Sprite::createWithTexture(avalon::physics::Box2dContainer& box2dContainer, cocos2d::Texture2D *texture)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithTexture(texture)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->createBody();
    sprite->addTextureShape();
    return sprite;
}

Sprite* Sprite::createWithTexture(avalon::physics::Box2dContainer& box2dContainer, cocos2d::Texture2D *texture, const cocos2d::Rect& rect)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithTexture(texture, rect)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->createBody();
    sprite->addTextureShape();
    return sprite;
}

Sprite* Sprite::createWithSpriteFrame(avalon::physics::Box2dContainer& box2dContainer, cocos2d::SpriteFrame *pSpriteFrame)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithSpriteFrame(pSpriteFrame)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->createBody();
    sprite->addTextureShape();
    return sprite;
}

Sprite* Sprite::createWithSpriteFrameName(avalon::physics::Box2dContainer& box2dContainer, const char *spriteFrameName)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithSpriteFrameName(spriteFrameName)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->createBody();
    sprite->addTextureShape();
    return sprite;
}

Sprite* Sprite::createWithPESShape(avalon::physics::Box2dContainer& box2dContainer, const char *filename, const std::string& pesFile, const std::string& pesShape)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->createBodyWithPESShape(pesFile, pesShape);
    return sprite;
}

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
    box2dContainer = config.box2dContainer;
    createBody(config.settings);

    float x = getPositionX();
    float y = getPositionY();
    setPosition({x, y});

    loadConfigurationSettings(config.settings);
}

void Sprite::onConfiguration(const avalon::io::TiledMapLoader::Configuration& config)
{
    box2dContainer = config.box2dContainer;
    createBody(config.settings);

    auto x = boost::any_cast<float>(config.settings.at("x"));
    auto y = boost::any_cast<float>(config.settings.at("y"));
    auto pos = avalon::utils::tiled::getPositionFromCoord(config.map, x, y);
    setPosition(pos);

    loadConfigurationSettings(config.settings);
}

void Sprite::loadConfigurationSettings(const std::map<std::string, boost::any>& settings)
{
    if (settings.count("friction")) {
        auto value = boost::any_cast<float>(settings.at("friction"));
        getBody().GetFixtureList()->SetFriction(value);
    }

    if (settings.count("density")) {
        auto value = boost::any_cast<float>(settings.at("density"));
        getBody().GetFixtureList()->SetDensity(value);
    }

    if (settings.count("restitution")) {
        auto value = boost::any_cast<float>(settings.at("restitution"));
        getBody().GetFixtureList()->SetRestitution(value);
    }

    if (settings.count("bodytype")) {
        auto value = boost::any_cast<std::string>(settings.at("bodytype"));
        getBody().SetType(getBox2dBodyType(value));
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

void Sprite::addTextureShape()
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

void Sprite::createBody()
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    body = box2dContainer->createBody(bodyDef, *this);
}

void Sprite::createBody(const std::map<std::string, boost::any>& settings)
{
    if (settings.count("pes.file")) {
        if (!settings.count("pes.shape")) {
            throw std::invalid_argument("pes.shape not defined!");
        }

        auto file = boost::any_cast<std::string>(settings.at("pes.file"));
        auto shape = boost::any_cast<std::string>(settings.at("pes.shape"));
        createBodyWithPESShape(file, shape);
    } else {
        createBody();
        addTextureShape();
    }
}

void Sprite::createBodyWithPESShape(const std::string& file, const std::string& shape)
{
    if (!hasBody()) {
        createBody();
    }

    auto cache = cocos2d::GB2ShapeCache::sharedGB2ShapeCache();
    cache->addShapesWithFile(file);
    cache->addFixturesToBody(&getBody(), shape);
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
