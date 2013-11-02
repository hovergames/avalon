#ifndef AVALON_PHYSICS_LOADERCALLBACKS_H
#define AVALON_PHYSICS_LOADERCALLBACKS_H

#include <avalon/io/TiledMapLoader.h>
#include <Box2D/Box2D.h>

namespace avalon {
namespace physics {
namespace loadercallbacks {

avalon::io::TiledMapLoader::Callback createShapes(int filterCategory = 0x0001, bool isSensor = false);

} // namespace loadercallbacks
} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_LOADERCALLBACKS_H */