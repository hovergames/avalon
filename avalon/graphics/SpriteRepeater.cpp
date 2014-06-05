#include "SpriteRepeater.h"

using namespace cocos2d;

namespace avalon {
namespace graphics {

SpriteRepeater::SpriteRepeater()
{
    resetToDefaults();
}

void SpriteRepeater::resetToDefaults()
{
    width = height = 0;
    paddingX = paddingY = -1;
    flipHorizontal = flipVertical = false;
    repeatHorizontal = repeatVertical = true;
}

cocos2d::Node* SpriteRepeater::createSprites()
{
    auto rootNode = cocos2d::Node::create();

    auto node = cocos2d::Sprite::createWithSpriteFrameName(fileName);
    assert(node && "Couldnt load sprite!");

    auto textureWidth = node->getTextureRect().size.width;
    auto textureHeight = node->getTextureRect().size.height;

    int countX = static_cast<int>(ceil(width / textureWidth));
    int countY = static_cast<int>(ceil(height / textureHeight));

    if (!repeatHorizontal)
        countX = 0;

    if (!repeatVertical)
        countY = 0;

    int childCounter = 0;

    for (int x = 0; x <= countX; ++x) {
        for (int y = 0; y <= countY; ++y) {
            node = cocos2d::Sprite::createWithSpriteFrameName(fileName);
            node->setPosition({x * (textureWidth + paddingX), -y * (textureHeight + paddingY)});
            node->setTag(childCounter);
            ++childCounter;

            cocos2d::Vec2 anchorPoint(0.0, 0.0);
            if (flipHorizontal && x % 2 == 1) {
                node->setScaleX(-1);
                anchorPoint.x = 1.0;
            }

            if (flipVertical && y % 2 == 1) {
                node->setScaleY(-1);
                anchorPoint.y = 1.0;
            }

            node->setAnchorPoint(anchorPoint);

            rootNode->addChild(node);
        }
    }
    
    return rootNode;
}

} // namespace graphics
} // namespace avalon
