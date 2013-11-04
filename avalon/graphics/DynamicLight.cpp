#include <avalon/graphics/DynamicLight.h>

using namespace cocos2d;

namespace avalon {
namespace graphics {

const GLchar* vertexShader =
#include "shaders/pass.vsh"

const GLchar* shadowMapFragmentShader =
#include "shaders/shadowMap.fsh"

const GLchar* shadowRenderFragmentShader =
#include "shaders/shadowRender.fsh"

DynamicLight::~DynamicLight()
{
    CC_SAFE_RELEASE(shadowRenderShader);
    CC_SAFE_RELEASE(shadowMapShader);
    CC_SAFE_RELEASE(occlusionMap);
    CC_SAFE_RELEASE(shadowMap1D);
    CC_SAFE_RELEASE(finalShadowMap);
    CC_SAFE_RELEASE(shadowCasters);
    CC_SAFE_RELEASE(bakedShadowMap);
}

bool DynamicLight::init()
{
    if (!Node::init())
        return false;

    shadowMapShader = this->loadShader(vertexShader, shadowMapFragmentShader);
    shadowRenderShader = this->loadShader(vertexShader, shadowRenderFragmentShader);

    initOcclusionMap();
    initShadowMap1D();
    initFinalShadowMap();
    initBakedShadowMap();

    updateUniforms();

    return true;
}

void DynamicLight::initOcclusionMap()
{
    CC_SAFE_RELEASE(occlusionMap);

    occlusionMap = RenderTexture::create(lightSize, lightSize);
    occlusionMap->retain();
}

void DynamicLight::initShadowMap1D()
{
    CC_SAFE_RELEASE(shadowMap1D);

    // seems like 16 pixel is the minimum height of a texture (on ios)
    shadowMap1D = RenderTexture::create(lightSize, 16);
    shadowMap1D->retain();
}

void DynamicLight::initFinalShadowMap()
{
    CC_SAFE_RELEASE(finalShadowMap);

    finalSize = lightSize * upScale;

    finalShadowMap = RenderTexture::create(finalSize, finalSize);
    finalShadowMap->retain();
}

void DynamicLight::initBakedShadowMap()
{
    CC_SAFE_RELEASE(bakedShadowMap);

    finalSize = lightSize * upScale;

    bakedShadowMap = RenderTexture::create(finalSize, finalSize);
    bakedShadowMap->retain();
}

void DynamicLight::setShadowCasters(Node* casters)
{
    this->shadowCasters = casters;
    this->shadowCasters->retain();
}

void DynamicLight::updateShadowMap()
{
    this->createOcclusionMap();
    this->createShadowMap();
}

void DynamicLight::setPosition(const Point& position)
{
    Node::setPosition(position);
    this->invalidateBakedMap();
}

void DynamicLight::draw()
{
    if (!bakedMapIsValid) {
        if (dirty) {
            updateUniforms();
            dirty = false;
        }

        updateUniforms();
        updateShadowMap();

        finalShadowMap->getSprite()->setColor({color.r, color.g, color.b});
        finalShadowMap->getSprite()->setShaderProgram(shadowRenderShader);
        finalShadowMap->getSprite()->setAnchorPoint({0, 0});
        finalShadowMap->getSprite()->setPosition({0, 0});
        finalShadowMap->setScaleY(-1);

        bakedShadowMap->beginWithClear(0.0, 0.0, 0.0, 0.0);
        finalShadowMap->setAnchorPoint({0.5, 0.5});
        finalShadowMap->setPosition({0, 0});
        finalShadowMap->visit();
        bakedShadowMap->end();
        bakedShadowMap->setPosition({0, 0});

        if (additive)
            bakedShadowMap->getSprite()->setBlendFunc({GL_SRC_ALPHA, GL_ONE});

        bakedMapIsValid = true;
    }

    bakedShadowMap->visit();

    if (debugDrawEnabled)
        debugDraw();
}

void DynamicLight::invalidateBakedMap()
{
    ++updateCount;
    if (updateCount > updateFrequency)
    {
        updateCount = 0;
        bakedMapIsValid = false;
    }
}

void DynamicLight::debugDraw()
{
    //debug rendering
    float width = EGLView::getInstance()->getDesignResolutionSize().width;
    float height = EGLView::getInstance()->getDesignResolutionSize().height;

    occlusionMap->getSprite()->setColor({255, 0, 0});
    occlusionMap->setAnchorPoint({0, 0});
    occlusionMap->setPosition({width - lightSize/2 - getPositionX(), height - lightSize/2 - getPositionY()});
    occlusionMap->visit();
    occlusionMap->getSprite()->setColor({255, 255, 255});

    shadowMap1D->setAnchorPoint({0, 0});
    shadowMap1D->setPosition({width - lightSize/2 - getPositionX(), height - lightSize - 15 - getPositionY()});
    shadowMap1D->visit();
}

GLProgram* DynamicLight::loadShader(const GLchar* vertexShader, const GLchar* fragmentShader)
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

void DynamicLight::updateUniforms()
{
    shadowMapShader->use();
    GLint res = shadowMapShader->getUniformLocationForName("resolution");
    shadowMapShader->setUniformLocationWith2f(res, lightSize, lightSize);

    GLint glUpscale = shadowMapShader->getUniformLocationForName("upScale");
    shadowMapShader->setUniformLocationWith1f(glUpscale, upScale);

    GLint glAccuracy = shadowMapShader->getUniformLocationForName("accuracy");
    shadowMapShader->setUniformLocationWith1f(glAccuracy, accuracy);

    shadowRenderShader->use();
    GLint glResolution2 = shadowRenderShader->getUniformLocationForName("resolution");
    GLint glSoftShadows = shadowRenderShader->getUniformLocationForName("softShadows");

    shadowRenderShader->setUniformLocationWith2f(glResolution2, lightSize, lightSize);
    shadowRenderShader->setUniformLocationWith1f(glSoftShadows, softShadows ? 1.0f : 0.0f);

    glUniform1i(glGetUniformLocation(shadowRenderShader->getProgram(), "u_texture2"), 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMap1D->getSprite()->getTexture()->getName());
    glActiveTexture(GL_TEXTURE0);
}

void DynamicLight::createOcclusionMap()
{
    if (!shadowCasters)
    {
        occlusionMap->beginWithClear(0.0 ,0.0, 0.0, 0.0);
        occlusionMap->end();
        return;
    }

    Point p1 = shadowCasters->getAnchorPoint();
    Point p2 = shadowCasters->getPosition();

    // Render light region to occluder FBO
    occlusionMap->beginWithClear(0.0, 0.0, 0.0, 0.0);
    shadowCasters->setAnchorPoint({0, 0});

    //ugly....
    shadowCasters->setPosition({-getPositionX() + lightSize/2 + shadowCasters->getPositionX(), -getPositionY() + lightSize/2 + shadowCasters->getPositionY()});
    shadowCasters->visit();
    occlusionMap->end();

    shadowCasters->setAnchorPoint(p1);
    shadowCasters->setPosition(p2);
}

void DynamicLight::createShadowMap()
{
    //STEP 2. build a 1D shadow map from occlude FBO
    occlusionMap->getSprite()->setShaderProgram(shadowMapShader);
    shadowMap1D->beginWithClear(0.0, 0.0, 0.0, 0.0);
    occlusionMap->setAnchorPoint({0.5, 0.5});
    occlusionMap->setPosition({lightSize / 2.0, lightSize / 2.0});
    occlusionMap->visit();
    shadowMap1D->end();
}

void DynamicLight::setSoftShadows(bool shadows)
{
    if (softShadows != shadows) {
        dirty = true;
        softShadows = shadows;
    }
}

void DynamicLight::setLightSize(int size)
{
    if (lightSize != size) {
        dirty = true;
        lightSize = size;
    }
}

void DynamicLight::setUpScale(float upScale)
{
    if (this->upScale != upScale) {
        dirty = true;
        this->upScale = upScale;
    }
}

void DynamicLight::setAccuracy(float accuracy)
{
    if (this->accuracy != accuracy) {
        dirty = true;
        this->accuracy = accuracy;
    }
}

void DynamicLight::setAdditive(bool additive)
{
    this->additive = additive;
}

void DynamicLight::setColor(const Color4B& c)
{
    this->color = c;
}

} // namespace physics
} // namespace avalon
