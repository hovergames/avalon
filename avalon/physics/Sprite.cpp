#include <avalon/physics/Sprite.h>

#include <avalon/utils/tiled.h>
#include <avalon/physics/Box2dContainer.h>

namespace avalon {
namespace physics {

void Sprite::onConfiguration(const avalon::io::CCBLoader::Configuration& config)
{
}

void Sprite::onConfiguration(const avalon::io::TiledMapLoader::Configuration& config)
{
    float x = std::stoi(config.settings.at("x"));
    float y = std::stoi(config.settings.at("y"));
    float w = getContentSize().width;
    float h = getContentSize().height;

    auto pos = avalon::utils::tiled::getPositionFromCoord(config.map, x, y);
    cocos2d::Sprite::setPosition(pos);
    pos.x -= w * getAnchorPoint().x;
    pos.y -= h * getAnchorPoint().y;

    // create the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((pos.x + (w / 2.0f)) / config.box2dContainer->pixelsInMeter,
                         (pos.y + (h / 2.0f)) / config.box2dContainer->pixelsInMeter);
    auto body = config.box2dContainer->createBody(bodyDef, *this);

    // define the shape
    b2PolygonShape shape;
    shape.SetAsBox((w / config.box2dContainer->pixelsInMeter) * 0.5f,
                   (h / config.box2dContainer->pixelsInMeter) * 0.5f);

    // create the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
}

} // namespace physics
} // namespace avalon
