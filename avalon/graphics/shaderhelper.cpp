#include <avalon/graphics/shaderhelper.h>

namespace {

const GLchar* whiteShader =
#include <avalon/graphics/shaders/white.fsh>

const GLchar* vertexShader =
#include <avalon/graphics/shaders/pass.vsh>

const GLchar* grassShader =
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

GLProgram* getWhiteShader()
{
    return loadShader(vertexShader, whiteShader);
}

GLProgram* getGrassShader()
{
    return loadShader(vertexShader, grassShader);
}

GLProgram* getDefaultShader()
{
    return ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE);
}

} // namespace physics
} // namespace avalon
