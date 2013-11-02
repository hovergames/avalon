#ifndef AVALON_PHYSICS_LOADERCALLBACKS_H
#define AVALON_PHYSICS_LOADERCALLBACKS_H

#include <avalon/io/TiledMapLoader.h>
#include <Box2D/Box2D.h>

namespace avalon {
namespace physics {
namespace loadercallbacks {

avalon::io::TiledMapLoader::Callback createShapes(int filterCategory = 0, bool isSensor = false);

std::shared_ptr<b2PolygonShape> initRectangleShape(float width, float height, float pixelsInMeter);
std::shared_ptr<b2ChainShape> initChainShape(std::list<cocos2d::Point> points, float pixelsInMeter, bool loop = false);
std::shared_ptr<b2EdgeShape> initEdgeShape(cocos2d::Point p1, cocos2d::Point p2, float pixelsInMeter);

} // namespace loadercallbacks
} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_LOADERCALLBACKS_H */