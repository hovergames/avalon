#ifndef AVALON_PHYSICS_JSONLOADER_H
#define AVALON_PHYSICS_JSONLOADER_H

#include <boost/noncopyable.hpp>
#include <avalon/physics/Box2dContainer.h>
#include <avalon/physics/vendors/b2dJson/b2dJson.h>

class b2Body;
class b2dJsonImage;
class b2Vec2;
namespace cocos2d { class Sprite; }
namespace avalon { namespace physics { class Sprite; } }
namespace avalon { namespace physics { class Node; } }

namespace avalon {
namespace physics {

class JsonLoader : public boost::noncopyable
{
public:
    using SpriteList = std::vector<Sprite*>;
    struct Configuration {
        JsonLoader& loader;
        const std::string& filename;
        const std::string& name;
        avalon::physics::Box2dContainer& box2dContainer;
        const SpriteList& sprites;
    };

private:
    using Assigner = std::function<void(Node&)>;
    using AssignerList = std::list<Assigner>;
    using Callback = std::function<void(const std::string& name)>;
    using CallbackList = std::list<Callback>;

    std::shared_ptr<b2dJson> json;
    std::string filename;
    Box2dContainer& box2dContainer;
    std::map<b2Body*, SpriteList> bodySprites;
    std::map<std::string, AssignerList> bodyNameAssigner;
    std::map<std::string, CallbackList> bodyNameFactories;

    void readFromString(const std::string& filename);
    void createSprites();
    void triggerFactories();
    cocos2d::Sprite& createPhysicsSprites(b2dJsonImage& def);
    cocos2d::Sprite& createStaticSprites(b2dJsonImage& def);
    void applyCommonSettings(b2dJsonImage& def, cocos2d::Sprite& sprite);
    
public:
    JsonLoader(Box2dContainer& box2dContainer, const std::string& filename);
    std::shared_ptr<b2dJson> load();
    
    std::shared_ptr<b2dJson> getJson();
    void moveAllBy(const b2Vec2& offset);

    template<typename T>
    void registerTypeForBodyName(const std::string& name)
    {
        if (bodyNameFactories.count(name)) {
            throw new std::invalid_argument("Name already registered");
        }

        bodyNameFactories[name].push_back([this](const std::string& name){
            Configuration configNoSprites{*this, filename, name, box2dContainer, SpriteList()};
            std::vector<b2Body*> bodies;
            getJson()->getBodiesByName(name, bodies);

            for (auto& body : bodies) {
                T* obj = T::create(box2dContainer, *body);
                box2dContainer.addChild(obj);

                if (!bodySprites.count(body)) {
                    obj->onConfiguration(configNoSprites);
                } else {
                    Configuration config{*this, filename, name, box2dContainer, bodySprites[body]};
                    obj->onConfiguration(config);
                    bodySprites.erase(body);
                }

                for (auto& cb : bodyNameAssigner[name]) {
                    cb(*obj);
                }
            }
        });
    }

    template<typename T>
    void assignObjectsByBodyName(const std::string& name, std::list<T*>& list)
    {
        if (!bodyNameFactories.count(name)) {
            throw new std::runtime_error("Name not registered yet");
        }

        bodyNameAssigner[name].push_back([&list](Node& node) {
            auto target = dynamic_cast<T*>(&node);
            if (!target) {
                throw new std::invalid_argument("Wrong Object type");
            }
            list.push_back(target);
        });
    }
};

} // namespace physics
} // namespace avalon

#endif /* AVALON_PHYSICS_JSONLOADER_H */