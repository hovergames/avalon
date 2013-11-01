#ifndef AVALON_PHYSICS_SPRITE_H
#define AVALON_PHYSICS_SPRITE_H

#include "cocos2d.h"
#include <avalon/io/CCBLoader.h>
#include <avalon/io/TiledMapLoader.h>

namespace avalon {
namespace physics {

class Sprite : public cocos2d::Sprite
{
public:
    void onConfiguration(const avalon::io::CCBLoader::Configuration& config);
    void onConfiguration(const avalon::io::TiledMapLoader::Configuration& config);
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_SPRITE_H */