#ifndef AVALON_PHYSICS_SPRITE_H
#define AVALON_PHYSICS_SPRITE_H

#include "cocos2d.h"
#include <avalon/io/CCBLoader.h>
#include <avalon/io/TiledMapLoader.h>

class b2Body;
namespace avalon { namespace physics { class Box2dContainer; } }

namespace avalon {
namespace physics {

class Sprite : public cocos2d::Sprite
{
private:
    b2Body* body = nullptr;
    avalon::physics::Box2dContainer* box2dContainer = nullptr;

public:
    ~Sprite();
    void onConfiguration(const avalon::io::CCBLoader::Configuration& config);
    void onConfiguration(const avalon::io::TiledMapLoader::Configuration& config);

    bool hasBody() const;
    b2Body& getBody();
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_SPRITE_H */