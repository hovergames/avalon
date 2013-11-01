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
    if (config.settings.count("pes.file"))
    {
        if (!config.settings.count("pes.shape")) throw std::invalid_argument("pes.shape not defined!");
        addFixtureToBodyFromPESFile(boost::any_cast<std::string>(config.settings.at("pes.file")), boost::any_cast<std::string>(config.settings.at("pes.shape")), *config.box2dContainer);
    } else {
        createBody(*config.box2dContainer);
        createDefaultShape();
    }

    float x = getPositionX();
    float y = getPositionY();
    setPosition({x, y});

    if (config.settings.count("friction"))
        getBody().GetFixtureList()->SetFriction(boost::any_cast<float>(config.settings.at("friction")));

    if (config.settings.count("density"))
        getBody().GetFixtureList()->SetDensity(boost::any_cast<float>(config.settings.at("density")));

    if (config.settings.count("restitution"))
        getBody().GetFixtureList()->SetRestitution(boost::any_cast<float>(config.settings.at("restitution")));

    if (config.settings.count("bodytype"))
        getBody().SetType(getBox2dBodyType(boost::any_cast<std::string>(config.settings.at("bodytype"))));
}

void Sprite::onConfiguration(const avalon::io::TiledMapLoader::Configuration& config)
{
    if (config.settings.count("pes.file"))
    {
        if (!config.settings.count("pes.shape")) throw std::invalid_argument("pes.shape not defined!");
        addFixtureToBodyFromPESFile(config.settings.at("pes.file"), config.settings.at("pes.shape"), *config.box2dContainer);
    } else {
        createBody(*config.box2dContainer);
        createDefaultShape();
    }

    float x = std::stoi(config.settings.at("x"));
    float y = std::stoi(config.settings.at("y"));
    auto pos = avalon::utils::tiled::getPositionFromCoord(config.map, x, y);
    setPosition(pos);

    if (config.settings.count("friction"))
        getBody().GetFixtureList()->SetFriction(std::stof(config.settings.at("friction")));

    if (config.settings.count("density"))
        getBody().GetFixtureList()->SetDensity(std::stof(config.settings.at("density")));

    if (config.settings.count("restitution"))
        getBody().GetFixtureList()->SetRestitution(std::stof(config.settings.at("restitution")));

    if (config.settings.count("bodytype"))
        getBody().SetType(getBox2dBodyType(config.settings.at("bodytype")));
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

    b2PolygonShape shape;
    shape.SetAsBox((w / box2dContainer->pixelsInMeter) * 0.5f,
                   (h / box2dContainer->pixelsInMeter) * 0.5f);

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

void Sprite::addFixtureToBodyFromPESFile(std::string pesFileName, std::string shapeName, Box2dContainer& box2DContainer)
{
    if (!hasBody())
        createBody(box2DContainer);

    cocos2d::GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile(pesFileName);
    cocos2d::GB2ShapeCache::sharedGB2ShapeCache()->addFixturesToBody(&getBody(), shapeName);
    setAnchorPoint(cocos2d::GB2ShapeCache::sharedGB2ShapeCache()->anchorPointForShape(shapeName));
}

bool Sprite::init(void)
{
    if (!cocos2d::Sprite::init())
        return false;
    
    scheduleUpdate();
    return true;
}

void Sprite::update(float delta)
{
    cocos2d::Sprite::update(delta);

    if (hasBody()) {
        float px = body->GetPosition().x * box2dContainer->pixelsInMeter;
        float py = body->GetPosition().y * box2dContainer->pixelsInMeter;
        float rotation = body->GetAngle();

        cocos2d::Sprite::setPosition({px, py});
        cocos2d::Sprite::setRotation(-CC_RADIANS_TO_DEGREES(rotation));
    }
}

void Sprite::setPosition(const cocos2d::Point& pos)
{
    cocos2d::Sprite::setPosition(pos);

    if (hasBody())
        getBody().SetTransform(b2Vec2(pos.x / box2dContainer->pixelsInMeter, pos.y / box2dContainer->pixelsInMeter), getBody().GetTransform().q.GetAngle());
}

} // namespace physics
} // namespace avalon
