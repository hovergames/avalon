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
    b2Body* body = nullptr;
    avalon::physics::Box2dContainer* box2dContainer = nullptr;

    void createBody(Box2dContainer& box2DContainer);
    void createDefaultShape();
    b2BodyType getBox2dBodyType(const std::string& type);

public:
    CREATE_FUNC(Sprite);
    
    ~Sprite();
    void onConfiguration(const avalon::io::CCBLoader::Configuration& config);
    void onConfiguration(const avalon::io::TiledMapLoader::Configuration& config);

    void addFixtureToBodyFromPESFile(const std::string& file, const std::string& shape, Box2dContainer& box2DContainer);

    bool hasBody() const;
    b2Body& getBody();

    virtual bool init() override;
    virtual void update(float delta) override;
    virtual void setPosition(const cocos2d::Point& pos) override;
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_SPRITE_H */