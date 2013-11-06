#include <avalon/physics/Sprite.h>

#include <avalon/utils/tiled.h>
#include <avalon/physics/Box2dContainer.h>
#include <avalon/physics/vendors/GB2ShapeCache.h>
#include <avalon/physics/utils.h>
#include <avalon/io/CCBLoader.h>
#include <avalon/io/TiledMapLoader.h>

namespace avalon {
namespace physics {

Sprite* Sprite::create(avalon::physics::Box2dContainer& box2dContainer, const std::string& filename)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename)) return nullptr;
    sprite->createBody(box2dContainer);
    sprite->addTextureShapeFixture();
    return sprite;
}

Sprite* Sprite::create(avalon::physics::Box2dContainer& box2dContainer, const std::string& filename, const cocos2d::Rect& rect)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename, rect)) return nullptr;
    sprite->createBody(box2dContainer);
    sprite->addTextureShapeFixture();
    return sprite;
}

Sprite* Sprite::createWithTexture(avalon::physics::Box2dContainer& box2dContainer, cocos2d::Texture2D *texture)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithTexture(texture)) return nullptr;
    sprite->createBody(box2dContainer);
    sprite->addTextureShapeFixture();
    return sprite;
}

Sprite* Sprite::createWithTexture(avalon::physics::Box2dContainer& box2dContainer, cocos2d::Texture2D *texture, const cocos2d::Rect& rect)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithTexture(texture, rect)) return nullptr;
    sprite->createBody(box2dContainer);
    sprite->addTextureShapeFixture();
    return sprite;
}

Sprite* Sprite::createWithSpriteFrame(avalon::physics::Box2dContainer& box2dContainer, cocos2d::SpriteFrame *pSpriteFrame)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithSpriteFrame(pSpriteFrame)) return nullptr;
    sprite->createBody(box2dContainer);
    sprite->addTextureShapeFixture();
    return sprite;
}

Sprite* Sprite::createWithSpriteFrameName(avalon::physics::Box2dContainer& box2dContainer, const std::string& spriteFrameName)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithSpriteFrameName(spriteFrameName)) return nullptr;
    sprite->createBody(box2dContainer);
    sprite->addTextureShapeFixture();
    return sprite;
}

Sprite* Sprite::createWithPESShape(avalon::physics::Box2dContainer& box2dContainer, const std::string& filename, const std::string& pesFile, const std::string& pesShape)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename)) return nullptr;
    sprite->createBody(box2dContainer);
    sprite->addPESShapeFixture(pesFile, pesShape);
    return sprite;
}

Sprite* Sprite::create(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const std::string& filename)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename)) return nullptr;
    sprite->bodyImpl.reset(new Body(box2dContainer, body));
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

Sprite* Sprite::create(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const std::string& filename, const cocos2d::Rect& rect)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename, rect)) return nullptr;
    sprite->bodyImpl.reset(new Body(box2dContainer, body));
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

Sprite* Sprite::createWithTexture(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::Texture2D *texture)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithTexture(texture)) return nullptr;
    sprite->bodyImpl.reset(new Body(box2dContainer, body));
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

Sprite* Sprite::createWithTexture(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::Texture2D *texture, const cocos2d::Rect& rect)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithTexture(texture, rect)) return nullptr;
    sprite->bodyImpl.reset(new Body(box2dContainer, body));
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

Sprite* Sprite::createWithSpriteFrame(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::SpriteFrame *pSpriteFrame)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithSpriteFrame(pSpriteFrame)) return nullptr;
    sprite->bodyImpl.reset(new Body(box2dContainer, body));
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

Sprite* Sprite::createWithSpriteFrameName(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const std::string& spriteFrameName)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithSpriteFrameName(spriteFrameName)) return nullptr;
    sprite->bodyImpl.reset(new Body(box2dContainer, body));
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

void Sprite::cleanup()
{
    if (ownsBody) {
        getBody().GetWorld()->DestroyBody(&getBody());
    }
    getBox2dContainer().removeNode(*this);

    cocos2d::Sprite::cleanup();
}

void Sprite::onConfiguration(const avalon::io::CCBLoader::Configuration& config)
{
    createBody(*config.box2dContainer, config.settings);

    float x = getPositionX();
    float y = getPositionY();
    setPosition({x, y});

    loadConfigurationSettings(config.settings);
}

void Sprite::onConfiguration(const avalon::io::TiledMapLoader::Configuration& config)
{
    createBody(*config.box2dContainer, config.settings);

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
        using avalon::physics::utils::getBodyTypeFromString;
        auto value = boost::any_cast<std::string>(settings.at("bodytype"));
        getBody().SetType(getBodyTypeFromString(value));
    }
}

void Sprite::createBody(Box2dContainer& box2dContainer)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    auto body = box2dContainer.createBody(bodyDef, *this);
    ownsBody = true;

    bodyImpl.reset(new Body(box2dContainer, *body));
}

void Sprite::createBody(Box2dContainer& box2dContainer, const std::map<std::string, boost::any>& settings)
{
    createBody(box2dContainer);

    if (settings.count("pes.file")) {
        if (!settings.count("pes.shape")) {
            throw std::invalid_argument("pes.shape not defined!");
        }

        auto file = boost::any_cast<std::string>(settings.at("pes.file"));
        auto shape = boost::any_cast<std::string>(settings.at("pes.shape"));
        addPESShapeFixture(file, shape);
    } else {
        addTextureShapeFixture();
    }
}

void Sprite::addPESShapeFixture(const std::string& file, const std::string& shape)
{
    auto cache = cocos2d::GB2ShapeCache::sharedGB2ShapeCache();
    cache->addShapesWithFile(file);
    cache->addFixturesToBody(&getBody(), shape);
}

void Sprite::addTextureShapeFixture()
{
    float w = getContentSize().width;
    float h = getContentSize().height;

    w = (w / getBox2dContainer().pixelsInMeter) * 0.5f;
    h = (h / getBox2dContainer().pixelsInMeter) * 0.5f;

    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    getBody().CreateFixture(&fixtureDef);
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

    bodyImpl->sync(this);
    cocos2d::Node::setAnchorPoint(bodyImpl->getAnchorPoint());
    cocos2d::Node::setRotation(bodyImpl->getRotation());
    cocos2d::Node::setPosition(bodyImpl->getPosition());
    cocos2d::Node::setScaleX(bodyImpl->getScaleX());
    cocos2d::Node::setScaleY(bodyImpl->getScaleY());
}

} // namespace physics
} // namespace avalon
