#include <avalon/utils/RepeatableSprite.h>
#include "cocos2d.h"
#include "ccUtils.h"

namespace avalon {
namespace utils {

RepeatableSprite* RepeatableSprite::create(const char *filename)
{
    auto *sprite = new RepeatableSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        sprite->initTextureRectAndPosition();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

RepeatableSprite* RepeatableSprite::create(const char *filename, const cocos2d::Rect& rect)
{
    auto sprite = new RepeatableSprite();
    if (sprite && sprite->initWithFile(filename, rect))
    {
        sprite->autorelease();
        sprite->initTextureRectAndPosition();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

RepeatableSprite* RepeatableSprite::createWithTexture(cocos2d::Texture2D *texture)
{
    auto sprite = new RepeatableSprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        sprite->initTextureRectAndPosition();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

RepeatableSprite* RepeatableSprite::createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect)
{
    auto sprite = new RepeatableSprite();
    if (sprite && sprite->initWithTexture(texture, rect))
    {
        sprite->autorelease();
        sprite->initTextureRectAndPosition();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

RepeatableSprite* RepeatableSprite::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
{
    auto sprite = new RepeatableSprite();
    if (spriteFrame && sprite && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        sprite->initTextureRectAndPosition();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

RepeatableSprite* RepeatableSprite::createWithSpriteFrameName(const char *spriteFrameName)
{
    auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);

#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName);
    CCASSERT(frame != NULL, msg);
#endif

    return createWithSpriteFrame(frame);
}

bool RepeatableSprite::initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated)
{
    cocos2d::Sprite::initWithTexture(texture, rect, rotated);
    initTextureRectAndPosition();
    return true;
}

void RepeatableSprite::initTextureRectAndPosition()
{
    auto size = cocos2d::EGLView::getInstance()->getDesignResolutionSize();
    setAnchorPoint({0, 0});
    setPosition({0, 0});
    setTextureRect(cocos2d::Rect(0, 0, size.width, size.height));

    auto tex = getTexture();
    if (tex) {
        cocos2d::Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };

        if (tex->getPixelsWide() != cocos2d::ccNextPOT(tex->getPixelsWide()) || tex->getPixelsHigh() != cocos2d::ccNextPOT(tex->getPixelsHigh())) {
            throw std::invalid_argument("RepeatableSprite Texture has to be a power of two!");
        }
        
        getTexture()->setTexParameters(params);
    }
}

void RepeatableSprite::moveTexture(const cocos2d::Point& point)
{
    float x = (_quad.tl.texCoords.u * getContentSize().width) + point.x;
    float y = (_quad.tl.texCoords.v * getContentSize().height) + point.y;

    setTexCoords({x, y});
}

void RepeatableSprite::setTextureOffset(const cocos2d::Point& point)
{
    setTexCoords({-point.x, point.y});
}

void RepeatableSprite::setTexCoords(const cocos2d::Point& point)
{
    float x = point.x / getContentSize().width;
    float y = point.y / getContentSize().height;
    float width = _quad.br.texCoords.u - _quad.tl.texCoords.u;
    float height = _quad.br.texCoords.v - _quad.tl.texCoords.v;

    _quad.tl.texCoords.u = x;
    _quad.tl.texCoords.v = y;

    _quad.tr.texCoords.u = _quad.tl.texCoords.u + width;
    _quad.tr.texCoords.v = _quad.tl.texCoords.v;

    _quad.bl.texCoords.u = _quad.tl.texCoords.u;
    _quad.bl.texCoords.v = _quad.tl.texCoords.v + height;

    _quad.br.texCoords.u = _quad.bl.texCoords.u + width;
    _quad.br.texCoords.v = _quad.tl.texCoords.v + height;
}

} // namespace utils
} // namespace avalon
