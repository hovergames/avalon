#include <avalon/physics/loadercallbacks.h>
#include "Box2D/Box2D.h"
#include <avalon/physics/Box2dContainer.h>

namespace avalon {
namespace physics {
namespace loadercallbacks {

void createShapes(const avalon::io::TiledMapLoader::Configuration& config)
{
    return;
    float x = std::atoi(config.settings.at("x").c_str());
    float y = std::atoi(config.settings.at("y").c_str());
    float w = std::atoi(config.settings.at("width").c_str());
    float h = std::atoi(config.settings.at("height").c_str());

    // create the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set((x + (w / 2.0f)) / config.box2dContainer->pixelsInMeter,
                         (y + (h / 2.0f)) / config.box2dContainer->pixelsInMeter);
    b2Body* body = config.box2dContainer->world->CreateBody(&bodyDef);

    // define the shape
    b2PolygonShape shape;
    shape.SetAsBox((w / config.box2dContainer->pixelsInMeter) * 0.5f,
                   (h / config.box2dContainer->pixelsInMeter) * 0.5f);

    // create the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
}


} // namesapce loadercallbacks
} // namespace physics
} // namespace avalon
