#ifndef AVALON_GRAPHICS_SHADERHELPER_H
#define AVALON_GRAPHICS_SHADERHELPER_H

#include "cocos2d.h"

namespace avalon {
namespace graphics {

cocos2d::GLProgram* loadShader(const GLchar* vertexShader, const GLchar* fragmentShader);
cocos2d::GLProgram* getFullWhiteShader();
cocos2d::GLProgram* getWhiteShader();
cocos2d::GLProgram* getGrassShader();
cocos2d::GLProgram* getDefaultShader();

} // namespace graphics
} // namespace avalon

#endif /* AVALON_GRAPHICS_SHADERHELPER_H */

