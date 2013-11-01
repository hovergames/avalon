#ifndef AVALON_UTILS_TILED_H
#define AVALON_UTILS_TILED_H

#include "cocos2d.h"

namespace avalon {
namespace utils {
namespace tiled {

cocos2d::Point getPositionFromCoord(const cocos2d::TMXTiledMap& map, int x, int y)
{
    // ??
    x += 0.5;

    // ??
    y += 1;

    // convert different y-axis between cocos2dx and tiled
    y = map.getMapSize().height - y;

    float sx = x * map.getTileSize().width;
    float sy = y * map.getTileSize().height;
    return {sx, sy};
}

} // namespace tiled
} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_TILED_H */
