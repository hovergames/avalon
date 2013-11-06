#include <avalon/physics/Sprite.h>

#include <avalon/utils/tiled.h>
#include <avalon/physics/Box2dContainer.h>
#include <avalon/physics/vendors/GB2ShapeCache.h>
#include <avalon/physics/utils.h>

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

Sprite* Sprite::create(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const char *filename)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->box2dContainer->assignNode(body, *sprite);
    sprite->body = &body;
    return sprite;
}

Sprite* Sprite::create(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const char *filename, const cocos2d::Rect& rect)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithFile(filename, rect)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->box2dContainer->assignNode(body, *sprite);
    sprite->body = &body;
    return sprite;
}

Sprite* Sprite::createWithTexture(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::Texture2D *texture)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithTexture(texture)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->box2dContainer->assignNode(body, *sprite);
    sprite->body = &body;
    return sprite;
}

Sprite* Sprite::createWithTexture(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::Texture2D *texture, const cocos2d::Rect& rect)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithTexture(texture, rect)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->box2dContainer->assignNode(body, *sprite);
    sprite->body = &body;
    return sprite;
}

Sprite* Sprite::createWithSpriteFrame(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::SpriteFrame *pSpriteFrame)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithSpriteFrame(pSpriteFrame)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->box2dContainer->assignNode(body, *sprite);
    sprite->body = &body;
    return sprite;
}

Sprite* Sprite::createWithSpriteFrameName(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const char *spriteFrameName)
{
    auto sprite = Sprite::create();
    if (!sprite || !sprite->initWithSpriteFrameName(spriteFrameName)) return nullptr;
    sprite->box2dContainer = &box2dContainer;
    sprite->box2dContainer->assignNode(body, *sprite);
    sprite->body = &body;
    return sprite;
}

void Sprite::cleanup()
{
    if (hasBody() && ownsBody) {
        getBody().GetWorld()->DestroyBody(&getBody());
    }
    if (box2dContainer) {
        box2dContainer->removeNode(*this);
    }
    cocos2d::Sprite::cleanup();
}

bool Sprite::hasBody() const
{
    return body;
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
        using avalon::physics::utils::getBodyTypeFromString;
        auto value = boost::any_cast<std::string>(settings.at("bodytype"));
        getBody().SetType(getBodyTypeFromString(value));
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
    ownsBody = true;
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

    cocos2d::Sprite::setAnchorPoint({0.5, 0.5});
    scheduleUpdate();

    return true;
}

void Sprite::update(float delta)
{
    cocos2d::Sprite::update(delta);

    if (!hasBody()) {
        return;
    }

    b2Vec2 box2PosWithOffset(positionOffset.x, positionOffset.y);
    b2Rot rot(body->GetAngle());
    box2PosWithOffset = b2Mul(rot, box2PosWithOffset) + body->GetPosition();

    auto pos = box2dContainer->convertFromBox2d(box2PosWithOffset);
    pos = box2dContainer->convertToWorldSpace(pos);
    pos = getParent()->convertToNodeSpace(pos);
    cocos2d::Sprite::setPosition({pos.x, pos.y});

    auto b = box2dContainer->getNodeToWorldTransform();
    auto bScaleX = sqrt(b.a * b.a + b.c * b.c);
    auto bScaleY = sqrt(b.b * b.b + b.d * b.d);
    auto bRotation = atan2(b.b, b.a);

    auto p = getParent()->getNodeToWorldTransform();
    auto pScaleX = sqrt(p.a * p.a + p.c * p.c);
    auto pScaleY = sqrt(p.b * p.b + p.d * p.d);
    auto pRotation = atan2(p.b, p.a);

    if (bScaleX == 0) bScaleX = 1;
    if (bScaleY == 0) bScaleY = 1;

    auto pScaleXAbs = pScaleX / bScaleX;
    auto pScaleYAbs = pScaleY / bScaleY;
    auto pRotationAbs = pRotation - bRotation;

    if (pScaleXAbs == 0) pScaleXAbs = 1;
    if (pScaleYAbs == 0) pScaleYAbs = 1;

    cocos2d::Sprite::setScaleX(myScaleX / pScaleXAbs);
    cocos2d::Sprite::setScaleY(myScaleY / pScaleYAbs);

    float rotation = body->GetAngle() - pRotationAbs;
    cocos2d::Sprite::setRotation(rotationOffset - CC_RADIANS_TO_DEGREES(rotation));
}

void Sprite::setPositionOffset(const cocos2d::Point& point)
{
    positionOffset = point;
}

const cocos2d::Point& Sprite::getPositionOffset()
{
    return positionOffset;
}

void Sprite::setRotationOffset(float rotation)
{
    rotationOffset = rotation;
}

float Sprite::getRotationOffset()
{
    return rotationOffset;
}

} // namespace physics
} // namespace avalon
