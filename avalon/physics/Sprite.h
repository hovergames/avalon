#ifndef AVALON_PHYSICS_SPRITE_H
#define AVALON_PHYSICS_SPRITE_H

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include <avalon/io/CCBLoader.h>
#include <avalon/io/TiledMapLoader.h>
#include <avalon/physics/Body.h>

namespace avalon { namespace physics { class Box2dContainer; } }

namespace avalon {
namespace physics {

class Sprite : public cocos2d::Sprite
{
private:
    std::shared_ptr<Body> bodyImpl;
    bool ownsBody = false;
    bool swallowSetAnchor = false;

    void syncBody();
    void resetBodyImpl(Box2dContainer& box2dContainer, b2Body& body);
    void createBody(Box2dContainer& box2dContainer);
    void createBody(Box2dContainer& box2dContainer, const std::map<std::string, boost::any>& settings);
    void addPESShapeFixture(const std::string& file, const std::string& shape);
    void addTextureShapeFixture();
    void loadConfigurationSettings(const std::map<std::string, boost::any>& settings);
    
    CREATE_FUNC(Sprite);

public:
    virtual bool init() override;
    virtual void cleanup() override;

    // delete "inherited" create methods
    static Sprite* create(const std::string& filename) = delete;
    static Sprite* create(const std::string& filename, const cocos2d::Rect& rect) = delete;
    static Sprite* createWithTexture(cocos2d::Texture2D *texture) = delete;
    static Sprite* createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect) = delete;
    static Sprite* createWithSpriteFrame(cocos2d::SpriteFrame *pSpriteFrame) = delete;
    static Sprite* createWithSpriteFrameName(const std::string& spriteFrameName) = delete;

    // and re-define them with the required Box2dContainer
    static Sprite* create(avalon::physics::Box2dContainer& box2dContainer, const std::string& filename);
    static Sprite* create(avalon::physics::Box2dContainer& box2dContainer, const std::string& filename, const cocos2d::Rect& rect);
    static Sprite* createWithTexture(avalon::physics::Box2dContainer& box2dContainer, cocos2d::Texture2D *texture);
    static Sprite* createWithTexture(avalon::physics::Box2dContainer& box2dContainer, cocos2d::Texture2D *texture, const cocos2d::Rect& rect);
    static Sprite* createWithSpriteFrame(avalon::physics::Box2dContainer& box2dContainer, cocos2d::SpriteFrame *pSpriteFrame);
    static Sprite* createWithSpriteFrameName(avalon::physics::Box2dContainer& box2dContainer, const std::string& spriteFrameName);

    // load a sprite next to a body without creating a new fixture at all
    static Sprite* create(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const std::string& filename);
    static Sprite* create(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const std::string& filename, const cocos2d::Rect& rect);
    static Sprite* createWithTexture(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::Texture2D *texture);
    static Sprite* createWithTexture(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::Texture2D *texture, const cocos2d::Rect& rect);
    static Sprite* createWithSpriteFrame(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::SpriteFrame *pSpriteFrame);
    static Sprite* createWithSpriteFrameName(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const std::string& spriteFrameName);

    // custom create methods
    static Sprite* createWithPESShape(avalon::physics::Box2dContainer& box2dContainer, const std::string& filename, const std::string& pesFile, const std::string& pesShape);

    void onConfiguration(const avalon::io::CCBLoader::Configuration& config);
    void onConfiguration(const avalon::io::TiledMapLoader::Configuration& config);

    virtual void update(float delta) override;

    // Manage swallowSetAnchor flag for all init methods
    virtual bool initWithTexture(cocos2d::Texture2D* texture) override;
    virtual bool initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect& rect) override;
    virtual bool initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect& rect, bool rotated) override;
    virtual bool initWithSpriteFrame(cocos2d::SpriteFrame* pSpriteFrame) override;
    virtual bool initWithSpriteFrameName(const std::string& spriteFrameName) override;
    virtual bool initWithFile(const std::string& filename) override;
    virtual bool initWithFile(const std::string& filename, const cocos2d::Rect& rect) override;

    // Redirect all important methods
    b2Body& getBody()                                                  { return bodyImpl->getBody(); }
    Box2dContainer& getBox2dContainer()                                { return bodyImpl->getBox2dContainer(); }
    virtual void setScaleX(float scaleX) override                      { bodyImpl->setScaleX(scaleX); }
    virtual void setScaleY(float scaleY) override                      { bodyImpl->setScaleY(scaleY); }
    virtual void setScale(float scaleX, float scaleY) override         { bodyImpl->setScale(scaleX, scaleY); }
    virtual void setScale(float scale) override                        { bodyImpl->setScale(scale); }
    virtual void setPosition(const cocos2d::Point& pos) override       { bodyImpl->setPosition(pos); }
    virtual void setPosition(float x, float y) override                { bodyImpl->setPosition(x, y); }
    virtual void setRotation(float rotation) override                  { bodyImpl->setRotation(rotation); }
    virtual void setRotationX(float rotationX) override                { bodyImpl->setRotationX(rotationX); }
    virtual void setRotationY(float rotationY) override                { bodyImpl->setRotationY(rotationY); }
    virtual void setAnchorPoint(const cocos2d::Point& anchor) override { if (!swallowSetAnchor) bodyImpl->setAnchorPoint(anchor); }
    virtual void ignoreAnchorPointForPosition(bool value) override     { bodyImpl->ignoreAnchorPointForPosition(value); }
    void setPositionOffset(const cocos2d::Point& point)                { bodyImpl->setPositionOffset(point); }
    void setRotationOffset(float rotation)                             { bodyImpl->setRotationOffset(rotation); }
    const cocos2d::Point& getPositionOffset()                          { return bodyImpl->getPositionOffset(); }
    float getRotationOffset()                                          { return bodyImpl->getRotationOffset(); }
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_SPRITE_H */