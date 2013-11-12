#ifndef AVALON_IO_CCBLOADER_PHYSICSPRITELOADER_H
#define AVALON_IO_CCBLOADER_PHYSICSPRITELOADER_H

#include "cocos2d.h"
#include <avalon/physics/Sprite.h>
#include <avalon/physics/Box2dContainer.h>
#include <avalon/io/ccbloader/GenericLoader.h>
#include <avalon/physics/utils.h>
#include <editor-support/cocosbuilder/CocosBuilder.h>

namespace {

// The PhysicSpriteLoader below is based on GenericLoader<T, L> and requires
// to call T::onConfiguration(). So we simply use this dummy to please the
// compiler. And no, it's not dangerous or something because in our loader
// below we ensure to call onConfiguration() on the real physics::Sprite.
// It looks a bit hacky but is totally fine ;)
class DummySprite : public cocos2d::Sprite
{
public:
    CREATE_FUNC(DummySprite);

    void onConfiguration(const avalon::io::CCBLoader::Configuration& config)
    {
    }
};

} // namespace

namespace avalon {
namespace io {
namespace ccbloader {

class PhysicSpriteLoader : public GenericLoader<DummySprite, cocosbuilder::SpriteLoader>
{
private:

    avalon::physics::Sprite* convertDummyToPhysicsSprite(avalon::physics::Box2dContainer& box2dContainer, DummySprite& node)
    {
        auto result = avalon::physics::Sprite::createWithTexture(box2dContainer, node.getTexture(), node.getTextureRect());

        // Copy all settings
        result->setDisplayFrame(node.getDisplayFrame());
        result->setOpacity(node.getOpacity());
        result->setFlippedX(node.isFlippedX());
        result->setFlippedY(node.isFlippedY());
        result->setColor(node.getColor());
        result->setBlendFunc(node.getBlendFunc());
        result->setTag(node.getTag());
        result->setZOrder(node.getZOrder());
        result->setPosition(node.getPositionX(), node.getPositionY());
        result->setContentSize(node.getContentSize());
        result->setSkewX(node.getSkewX());
        result->setSkewY(node.getSkewY());
        result->setScaleX(node.getScaleX());
        result->setScaleY(node.getScaleY());
        result->setRotation(node.getRotation());
        result->setVisible(node.isVisible());

        // Translate anchorPoint to positionOffset
        cocos2d::Point offset;
        offset.x = (node.getAnchorPoint().x - 0.5f) * node.getContentSize().width * node.getScaleX();
        offset.y = (node.getAnchorPoint().y - 0.5f) * node.getContentSize().height * node.getScaleY();
        result->setPositionOffset(offset);

        // Forward unsupported stuff only if different from the default
        // values. This will trigger errors only if the user has used them.
        if (node.getRotationX() != 0) result->setRotationX(node.getRotationX());
        if (node.getRotationY() != 0) result->setRotationY(node.getRotationY());
        if (node.isIgnoreAnchorPointForPosition())result->ignoreAnchorPointForPosition(node.isIgnoreAnchorPointForPosition());

        return result;
    }

public:
    virtual void dispatchPendingProperties(cocosbuilder::CCBAnimationManager& animationManager, avalon::physics::Box2dContainer* box2dContainer) override
    {
        for (auto& pair : nodeSettings) {
            auto& tempNode = pair.first;
            auto& settings = pair.second;
            
            auto realNode = convertDummyToPhysicsSprite(*box2dContainer, *tempNode);
            Configuration config{settings, animationManager, box2dContainer};
            realNode->onConfiguration(config);

            tempNode->getParent()->addChild(realNode);
            tempNode->removeFromParent();
        }
    }

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(PhysicSpriteLoader, loader);
};

} // namespace ccbloader
} // namespace io
} // namespace avalon

#endif /* AVALON_IO_CCBLOADER_PHYSICSPRITELOADER_H */