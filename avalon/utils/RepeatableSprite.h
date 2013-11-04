#ifndef AVALON_UTILS_REPEATABLESPRITE_H
#define AVALON_UTILS_REPEATABLESPRITE_H

#include "cocos2d.h"

namespace avalon {
namespace utils {

class RepeatableSprite : public cocos2d::Sprite
{
private:
    void setTexCoords(const cocos2d::Point& point);
    void initTextureRectAndPosition();

public:
    using cocos2d::Sprite::initWithTexture;

    CREATE_FUNC(RepeatableSprite);

    static RepeatableSprite* create(const char *filename);
    static RepeatableSprite* create(const char *filename, const cocos2d::Rect& rect);
    static RepeatableSprite* createWithTexture(cocos2d::Texture2D *texture);
    static RepeatableSprite* createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect);
    static RepeatableSprite* createWithSpriteFrame(cocos2d::SpriteFrame *pSpriteFrame);
    static RepeatableSprite* createWithSpriteFrameName(const char *spriteFrameName);
    bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated) override;

    void setTextureOffset(const cocos2d::Point& point);
    void moveTexture(const cocos2d::Point& point);
};

} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_REPEATABLESPRITE_H */
