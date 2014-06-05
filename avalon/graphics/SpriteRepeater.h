#ifndef AVALON_GRAPHICS_SPRITEREPEATER_H
#define AVALON_GRAPHICS_SPRITEREPEATER_H

#include "cocos2d.h"

namespace avalon {
namespace graphics {

class SpriteRepeater
{
public:
    std::string fileName;

    float width = 0;
    float height = 0;

    float paddingX = -1;
    float paddingY = -1;

    bool flipHorizontal = false;
    bool flipVertical = false;

    bool repeatHorizontal = true;
    bool repeatVertical = true;

    cocos2d::Node* createSprite();
};

} // namespace graphics
} // namespace avalon

#endif

