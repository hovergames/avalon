#ifndef AVALON_GRAPHICS_SPRITEREPEATER_H
#define AVALON_GRAPHICS_SPRITEREPEATER_H

#include "cocos2d.h"

namespace avalon {
namespace graphics {

class SpriteRepeater
{
public:
    std::string fileName;

    float width;
    float height;

    float paddingX;
    float paddingY;

    bool flipHorizontal;
    bool flipVertical;

    bool repeatHorizontal;
    bool repeatVertical;

    int vertexZ;

    SpriteRepeater();

    cocos2d::Node* createSprites();

    void resetToDefaults();
};

} // namespace graphics
} // namespace avalon

#endif

