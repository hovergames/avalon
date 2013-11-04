#include <avalon/utils/texture.h>

namespace avalon {
namespace utils {
namespace texture {

void setAntialias(cocos2d::Sprite& sprite, bool mode)
{
    setAntialias(*sprite.getTexture(), mode);
}

void setAntialias(cocos2d::Texture2D& tex, bool mode)
{
    if (mode) {
        tex.setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });
    } else {
        tex.setTexParameters({ GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT });
    }
}

} // namespace texture
} // namespace utils
} // namespace avalon
