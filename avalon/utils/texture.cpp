#include <avalon/utils/texture.h>
#include "cocos2d.h"

namespace avalon {
namespace utils {
namespace texture {

void setAntialias(cocos2d::Sprite *sprite, bool mode)
{
    setAntialias(sprite->getTexture(), mode);
}

void setAntialias(cocos2d::Texture2D *tex, bool mode)
{
    if (mode)
    {
        cocos2d::Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
        tex->setTexParameters(params);
    } else {
        cocos2d::Texture2D::TexParams params = { GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT };
        tex->setTexParameters(params);
    }
}

} // namespace texture
} // namespace utils
} // namespace avalon
