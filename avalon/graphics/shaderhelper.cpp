#include <avalon/graphics/shaderhelper.h>

namespace {

const GLchar* whiteShader =
#include "shaders/white.fsh"

const GLchar* vertexShader =
#include "shaders/pass.vsh"

}

namespace avalon {
namespace graphics {

using namespace cocos2d;

GLProgram* loadShader(const GLchar* vertexShader, const GLchar* fragmentShader)
{
    GLProgram* shader = ShaderCache::getInstance()->getProgram(fragmentShader);
    if (!shader) {
        shader = new GLProgram();
        shader->retain();
        shader->initWithVertexShaderByteArray(vertexShader, fragmentShader);
        shader->addAttribute(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
        shader->addAttribute(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
        shader->addAttribute(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
        shader->link();
        shader->updateUniforms();
        shader->use();
    }

    return shader;
}

GLProgram* getWhiteShader()
{
    return loadShader(vertexShader, whiteShader);
}

GLProgram* getDefaultShader()
{
    return ShaderCache::getInstance()->programForKey(kCCShader_PositionTexture);
}

} // namespace physics
} // namespace avalon
