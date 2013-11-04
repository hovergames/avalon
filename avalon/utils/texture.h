#ifndef AVALON_UTILS_TEXTUREHELPER_H
#define AVALON_UTILS_TEXTUREHELPER_H

#include "cocos2d.h"

namespace avalon {
namespace utils {
namespace texture {

void setAntialias(cocos2d::Sprite& sprite, bool mode);
void setAntialias(cocos2d::Texture2D& tex, bool mode);

}
} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_TEXTUREHELPER_H */
