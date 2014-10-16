#include <avalon/graphics/shaderhelper.h>

#define STRINGIFY(A) #A

namespace {

#include "shaders/white.fsh"

#include "shaders/fullWhite.fsh"

const char* grassShader =
#include <avalon/graphics/shaders/grass.fsh>

} // namespace

namespace avalon {
namespace graphics {

using namespace cocos2d;

GLProgram* loadShader(const GLchar* vertexShader, const GLchar* fragmentShader)
{
    auto shader = ShaderCache::getInstance()->getGLProgram(fragmentShader);
    if (!shader) {
        shader = new GLProgram();
        shader->initWithByteArrays(vertexShader, fragmentShader);
        shader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        shader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        shader->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        shader->link();
        shader->updateUniforms();
        shader->use();
    }

    return shader;
}

GLProgram* getFullWhiteShader()
{
    return loadShader(cocos2d::ccPositionTextureColor_noMVP_vert, fullWhiteShader);
}

GLProgram* getWhiteShader()
{
    return loadShader(cocos2d::ccPositionTextureColor_noMVP_vert, whiteShader);
}

GLProgram* getGrassShader()
{
    return loadShader(cocos2d::ccPositionTextureColor_noMVP_vert, grassShader);
}

GLProgram* getDefaultShader()
{
    return GLProgramCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
}

} // namespace physics
} // namespace avalon
