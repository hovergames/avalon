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
    sprite->resetBodyImpl(box2dContainer, body);
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

Sprite* Sprite::create(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const std::string& filename, const cocos2d::Rect& rect)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename, rect)) return nullptr;
    sprite->resetBodyImpl(box2dContainer, body);
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

Sprite* Sprite::createWithTexture(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::Texture2D *texture)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithTexture(texture)) return nullptr;
    sprite->resetBodyImpl(box2dContainer, body);
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

Sprite* Sprite::createWithTexture(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::Texture2D *texture, const cocos2d::Rect& rect)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithTexture(texture, rect)) return nullptr;
    sprite->resetBodyImpl(box2dContainer, body);
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

Sprite* Sprite::createWithSpriteFrame(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::SpriteFrame *pSpriteFrame)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithSpriteFrame(pSpriteFrame)) return nullptr;
    sprite->resetBodyImpl(box2dContainer, body);
    sprite->getBox2dContainer().assignNode(body, *sprite);
    return sprite;
}

Sprite* Sprite::createWithSpriteFrameName(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const std::string& spriteFrameName)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithSpriteFrameName(spriteFrameName)) return nullptr;
    sprite->resetBodyImpl(box2dContainer, body);
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
    loadConfigurationSettings(config.settings);
}

void Sprite::onConfiguration(const avalon::io::TiledMapLoader::Configuration& config)
{
    createBody(*config.box2dContainer, config.settings);

    auto x = config.settings.at("x").asFloat();
    auto y = config.settings.at("y").asFloat();
    auto pos = avalon::utils::tiled::getPositionFromCoord(config.map, x, y);
    setPosition(pos);

    loadConfigurationSettings(config.settings);
}

void Sprite::loadConfigurationSettings(const cocos2d::ValueMap& settings)
{
    if (settings.count("friction")) {
        auto value = settings.at("friction").asFloat();
        getBody().GetFixtureList()->SetFriction(value);
    }

    if (settings.count("density")) {
        auto value = settings.at("density").asFloat();
        getBody().GetFixtureList()->SetDensity(value);
    }

    if (settings.count("restitution")) {
        auto value = settings.at("restitution").asFloat();
        getBody().GetFixtureList()->SetRestitution(value);
    }

    if (settings.count("bodytype")) {
        using avalon::physics::utils::getBodyTypeFromString;
        auto value = settings.at("bodytype").asString();
        getBody().SetType(getBodyTypeFromString(value));
    }
}

void Sprite::resetBodyImpl(Box2dContainer& box2dContainer, b2Body& body)
{
    bodyImpl.reset(new Body(box2dContainer, body));
    syncBody();
}

void Sprite::createBody(Box2dContainer& box2dContainer)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    auto body = box2dContainer.createBody(bodyDef, *this);
    ownsBody = true;

    resetBodyImpl(box2dContainer, *body);
}

void Sprite::createBody(Box2dContainer& box2dContainer, const cocos2d::ValueMap& settings)
{
    if (bodyImpl) {
        bodyImpl->clearFixtures();
    } else {
        createBody(box2dContainer);
    }

    if (settings.count("pes.file")) {
        if (!settings.count("pes.shape")) {
            throw std::invalid_argument("pes.shape not defined!");
        }

        auto file = settings.at("pes.file").asString();
        auto shape = settings.at("pes.shape").asString();
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

void Sprite::scheduleUpdate()
{
    unscheduleUpdate();
    cocos2d::Sprite::scheduleUpdate();
}

bool Sprite::init()
{
    swallowSetAnchor = true;
    if (!cocos2d::Sprite::init()) {
        swallowSetAnchor = false;
        return false;
    }
    swallowSetAnchor = false;

    scheduleUpdate();
    return true;
}

bool Sprite::initWithTexture(cocos2d::Texture2D* texture)
{
    swallowSetAnchor = true;
    auto result = cocos2d::Sprite::initWithTexture(texture);
    swallowSetAnchor = false;
    return result;
}

bool Sprite::initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect& rect)
{
    swallowSetAnchor = true;
    auto result = cocos2d::Sprite::initWithTexture(texture, rect);
    swallowSetAnchor = false;
    return result;
}

bool Sprite::initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect& rect, bool rotated)
{
    swallowSetAnchor = true;
    auto result = cocos2d::Sprite::initWithTexture(texture, rect, rotated);
    swallowSetAnchor = false;
    return result;
}

bool Sprite::initWithSpriteFrame(cocos2d::SpriteFrame* pSpriteFrame)
{
    swallowSetAnchor = true;
    auto result = cocos2d::Sprite::initWithSpriteFrame(pSpriteFrame);
    swallowSetAnchor = false;
    return result;
}

bool Sprite::initWithSpriteFrameName(const std::string& spriteFrameName)
{
    swallowSetAnchor = true;
    auto result = cocos2d::Sprite::initWithSpriteFrameName(spriteFrameName);
    swallowSetAnchor = false;
    return result;
}

bool Sprite::initWithFile(const std::string& filename)
{
    swallowSetAnchor = true;
    auto result = cocos2d::Sprite::initWithFile(filename);
    swallowSetAnchor = false;
    return result;
}

bool Sprite::initWithFile(const std::string& filename, const cocos2d::Rect& rect)
{
    swallowSetAnchor = true;
    auto result = cocos2d::Sprite::initWithFile(filename, rect);
    swallowSetAnchor = false;
    return result;
}

void Sprite::update(float delta)
{
    syncBody();
    cocos2d::Sprite::update(delta);
}

void Sprite::syncBody()
{
    bodyImpl->sync(this);
    cocos2d::Node::setAnchorPoint(bodyImpl->getAnchorPoint());
    cocos2d::Node::setRotation(bodyImpl->getRotation());
    cocos2d::Node::setPosition(bodyImpl->getPosition());
    cocos2d::Node::setScaleX(bodyImpl->getScaleX());
    cocos2d::Node::setScaleY(bodyImpl->getScaleY());
}

} // namespace physics
} // namespace avalon
