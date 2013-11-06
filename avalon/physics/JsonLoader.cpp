#include <avalon/physics/JsonLoader.h>

#include "cocos2d.h"
#include <avalon/io/utils.h>
#include <avalon/physics/Sprite.h>
#include <avalon/physics/vendors/b2dJson/b2dJsonImage.h>

namespace avalon {
namespace physics {

JsonLoader::JsonLoader(Box2dContainer& box2dContainer, const std::string& filename)
: box2dContainer(box2dContainer)
, filename(filename)
{
}

std::shared_ptr<b2dJson> JsonLoader::load()
{
    if (getJson()) {
        throw new std::runtime_error("Already loaded");
    }

    readFromString(filename);
    createSprites();
    triggerFactories();

    return getJson();
}

void JsonLoader::triggerFactories()
{
    for (auto& factory : bodyNameFactories) {
        for (auto& callback : factory.second) {
            callback(factory.first);
        }
    }
}

void JsonLoader::readFromString(const std::string& filename)
{
    std::string error;
    auto data = avalon::io::utils::getStringFromFile(filename);

    json.reset(new b2dJson);
    json->readFromString(data, error, &box2dContainer.getWorld());

    if (error.length() > 0) {
        throw new std::runtime_error(error);
    }
}

void JsonLoader::createSprites()
{
    std::vector<b2dJsonImage*> b2dImages;
    getJson()->getAllImages(b2dImages);

    cocos2d::Sprite* sprite = nullptr;
    for (auto& def : b2dImages) {
        if (def->body) {
            sprite = &createPhysicsSprites(*def);
        } else {
            sprite = &createStaticSprites(*def);
        }

        applyCommonSettings(*def, *sprite);
        box2dContainer.addChild(sprite, def->renderOrder);
    }
}

cocos2d::Sprite& JsonLoader::createPhysicsSprites(b2dJsonImage& def)
{
    auto sprite = Sprite::create(box2dContainer, *def.body, def.file);
    if (!sprite) {
        throw new std::invalid_argument(std::string("Unable to load sprite: " + def.file));
    }

    sprite->setPositionOffset({def.center.x, def.center.y});
    sprite->setRotationOffset(CC_RADIANS_TO_DEGREES(-def.angle));

    bodySprites[def.body].push_back(sprite);
    return *sprite;
}

cocos2d::Sprite& JsonLoader::createStaticSprites(b2dJsonImage& def)
{
    auto sprite = cocos2d::Sprite::create(def.file);
    if (!sprite) {
        throw new std::invalid_argument(std::string("Unable to load sprite: " + def.file));
    }

    auto x = def.center.x * box2dContainer.pixelsInMeter;
    auto y = def.center.y * box2dContainer.pixelsInMeter;
    sprite->setPosition({x, y});
    sprite->setRotation(CC_RADIANS_TO_DEGREES(-def.angle));

    return *sprite;
}

void JsonLoader::applyCommonSettings(b2dJsonImage& def, cocos2d::Sprite& sprite)
{
    sprite.setFlippedX(def.flip);
    sprite.setColor({def.colorTint[0], def.colorTint[1], def.colorTint[2]});
    sprite.setOpacity(def.colorTint[3]);

    auto size = sprite.getContentSize();

    auto heightInPixel = def.scale * box2dContainer.pixelsInMeter;
    auto heightScale = heightInPixel / size.height;

    auto widthInPixel = size.width * heightScale * def.aspectScale;
    auto widthScale = widthInPixel / size.width;

    sprite.setScaleX(widthScale);
    sprite.setScaleY(heightScale);
}

std::shared_ptr<b2dJson> JsonLoader::getJson()
{
    return json;
}

void JsonLoader::moveAllBy(const b2Vec2& delta)
{
    if (!getJson()) {
        throw new std::runtime_error("Nothing loaded yet");
    }
    
    vector<b2Body*> bodies;
    getJson()->getAllBodies(bodies);

    for (auto& body : bodies) {
        body->SetTransform(body->GetPosition() + delta, body->GetAngle());
    }
}

} // namespace physics
} // namespace avalon