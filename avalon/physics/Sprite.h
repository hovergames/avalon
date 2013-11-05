#ifndef AVALON_PHYSICS_SPRITE_H
#define AVALON_PHYSICS_SPRITE_H

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include <avalon/io/CCBLoader.h>
#include <avalon/io/TiledMapLoader.h>

namespace avalon { namespace physics { class Box2dContainer; } }

namespace avalon {
namespace physics {

class Sprite : public cocos2d::Sprite
{
private:
    avalon::physics::Box2dContainer* box2dContainer = nullptr;
    b2Body* body = nullptr;
    bool ownsBody = false;
    cocos2d::Point center;

    void createBody();
    void createBody(const std::map<std::string, boost::any>& settings);
    void createBodyWithPESShape(const std::string& file, const std::string& shape);
    void addTextureShape();
    void loadConfigurationSettings(const std::map<std::string, boost::any>& settings);
    b2BodyType getBox2dBodyType(const std::string& type);

public:
    CREATE_FUNC(Sprite);
    virtual bool init() override;
    virtual void cleanup() override;

    // delete "inherited" create methods
    static Sprite* create(const char *filename) = delete;
    static Sprite* create(const char *filename, const cocos2d::Rect& rect) = delete;
    static Sprite* createWithTexture(cocos2d::Texture2D *texture) = delete;
    static Sprite* createWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect) = delete;
    static Sprite* createWithSpriteFrame(cocos2d::SpriteFrame *pSpriteFrame) = delete;
    static Sprite* createWithSpriteFrameName(const char *spriteFrameName) = delete;

    // and re-define them with the required Box2dContainer
    static Sprite* create(avalon::physics::Box2dContainer& box2dContainer, const char *filename);
    static Sprite* create(avalon::physics::Box2dContainer& box2dContainer, const char *filename, const cocos2d::Rect& rect);
    static Sprite* createWithTexture(avalon::physics::Box2dContainer& box2dContainer, cocos2d::Texture2D *texture);
    static Sprite* createWithTexture(avalon::physics::Box2dContainer& box2dContainer, cocos2d::Texture2D *texture, const cocos2d::Rect& rect);
    static Sprite* createWithSpriteFrame(avalon::physics::Box2dContainer& box2dContainer, cocos2d::SpriteFrame *pSpriteFrame);
    static Sprite* createWithSpriteFrameName(avalon::physics::Box2dContainer& box2dContainer, const char *spriteFrameName);

    // load a sprite next to a body without creating a new fixture at all
    static Sprite* create(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const char *filename);
    static Sprite* create(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const char *filename, const cocos2d::Rect& rect);
    static Sprite* createWithTexture(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::Texture2D *texture);
    static Sprite* createWithTexture(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::Texture2D *texture, const cocos2d::Rect& rect);
    static Sprite* createWithSpriteFrame(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, cocos2d::SpriteFrame *pSpriteFrame);
    static Sprite* createWithSpriteFrameName(avalon::physics::Box2dContainer& box2dContainer, b2Body& body, const char *spriteFrameName);

    // custom create methods
    static Sprite* createWithPESShape(avalon::physics::Box2dContainer& box2dContainer, const char *filename, const std::string& pesFile, const std::string& pesShape);

    void onConfiguration(const avalon::io::CCBLoader::Configuration& config);
    void onConfiguration(const avalon::io::TiledMapLoader::Configuration& config);

    bool hasBody() const;
    b2Body& getBody();

    void setCenter(const cocos2d::Point& point);
    const cocos2d::Point& getCenter();

    virtual void update(float delta) override;
    virtual void setPosition(const cocos2d::Point& pos) override;
    virtual void setRotation(float rotation) override;
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_SPRITE_H */