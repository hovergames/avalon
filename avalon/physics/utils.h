#ifndef AVALON_PHYSICS_UTILS_H
#define AVALON_PHYSICS_UTILS_H

#include <Box2D/Box2D.h>
#include <avalon/io/TiledMapLoader.h>

namespace avalon {
namespace physics {
namespace utils {

b2Vec2 convertToBox2d(const avalon::physics::Box2dContainer& box2dContainer, const cocos2d::Point& pos, const cocos2d::Size& size);
cocos2d::Point convertFromBox2d(const avalon::physics::Box2dContainer& box2dContainer, const b2Vec2& pos, const cocos2d::Size& size);
b2BodyType getBodyTypeFromString(const std::string& type);
avalon::io::TiledMapLoader::Callback shapeLoader(int filterCategory = 0x0001, bool isSensor = false);

} // namespace utils
} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_UTILS_H */