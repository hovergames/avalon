#ifndef AVALON_PHYSICS_LOADERCALLBACKS_H
#define AVALON_PHYSICS_LOADERCALLBACKS_H

#include <avalon/io/TiledMapLoader.h>

namespace avalon {
namespace physics {
namespace loadercallbacks {

void createShapes(const avalon::io::TiledMapLoader::Configuration& config);

} // namespace loadercallbacks
} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_LOADERCALLBACKS_H */