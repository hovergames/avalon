#ifndef AVALON_IO_TILEDMAPLOADER_H
#define AVALON_IO_TILEDMAPLOADER_H

#include "cocos2d.h"
#include <boost/any.hpp>
#include "avalon/io/CCBLoader.h"

namespace avalon { namespace physics { class Box2dContainer; } }

namespace avalon {
namespace io {

class TiledMapLoader
{
public:
    using Dictionary = ccbloader::Dictionary;
    struct Configuration
    {
        const Dictionary& settings;
        const std::string& layer;
        cocos2d::TMXTiledMap& map;
        avalon::physics::Box2dContainer* box2dContainer;
    };
    using Callback = std::function<void(const Configuration&)>;

private:
    using Assigner = std::function<void(cocos2d::Node&)>;
    using CallbackList = std::list<Callback>;
    using AssignerList = std::list<Assigner>;

    std::unordered_map<int, CallbackList> gidFactories;
    std::unordered_map<std::string, CallbackList> nameFactories;
    std::unordered_map<int, AssignerList> gidAssigner;
    std::unordered_map<std::string, AssignerList> nameAssigner;
    avalon::physics::Box2dContainer* box2dContainer = nullptr;
    const std::string mapFileName;

    bool isFiltered(const std::string& name, const std::list<std::string>& filter = {});
    void loadGidFactories(cocos2d::TMXTiledMap& map);
    void loadNamedFactories(cocos2d::TMXTiledMap& map);
    
public:
    explicit TiledMapLoader(const std::string& mapFileName);
    cocos2d::TMXTiledMap* load();
    void setBox2dContainer(avalon::physics::Box2dContainer& container);

    template<typename T>
    void assignObjectsByGID(const int gid, std::list<T*>& list)
    {
        if (!gidFactories.count(gid)) {
            throw new std::runtime_error("GID not registered yet");
        }

        gidAssigner[gid].push_back([&list](cocos2d::Node& node) {
            auto target = dynamic_cast<T*>(&node);
            if (!target) {
                throw new std::invalid_argument("Wrong Object type");
            }
            list.push_back(target);
        });
    }

    template<typename T>
    void assignObjectsByName(const std::string& name, std::list<T*>& list)
    {
        if (!nameFactories.count(name)) {
            throw new std::runtime_error("Name not registered yet");
        }

        nameAssigner[name].push_back([&list](cocos2d::Node& node) {
            auto target = dynamic_cast<T*>(&node);
            if (!target) {
                throw new std::invalid_argument("Wrong Object type");
            }
            list.push_back(target);
        });
    }

    void registerCallbackForGID(const int gid, const Callback& callback, const std::list<std::string>& layerFilter = {});
    void registerCallbackForName(const std::string& name, const Callback& callback, const std::list<std::string>& layerFilter = {});

    template<typename T>
    void registerObjectForGID(const int gid, T& object, const std::list<std::string>& layerFilter = {})
    {
        registerCallbackForGID(gid, [&object](const Configuration& config) {
            object->onConfiguration(config);
        }, layerFilter);
    }

    template<typename T>
    void registerObjectForName(const std::string& name, T& object, const std::list<std::string>& layerFilter = {})
    {
        registerCallbackForName(name, [&object](const Configuration& config) {
            object->onConfiguration(config);
        }, layerFilter);
    }

    template<typename T>
    void registerTypeForGID(const int gid, const std::list<std::string>& layerFilter = {})
    {
        gidFactories[gid].push_back([this, gid, layerFilter](const Configuration& config)
        {
            if (isFiltered(config.layer, layerFilter)) {
                return;
            }

            auto newObject = T::create();
            newObject->onConfiguration(config);
            config.map.addChild(newObject);

            if (!gidAssigner.count(gid)) {
                return;
            }

            for (auto& callback : gidAssigner.at(gid)) {
                callback(*newObject);
            }
        });
    }

    template<typename T>
    void registerTypeForName(const std::string& name, const std::list<std::string>& layerFilter = {})
    {
        nameFactories[name].push_back([this, name, layerFilter](const Configuration& config)
        {
            if (isFiltered(config.layer, layerFilter)) {
                return;
            }

            auto newObject = T::create();
            newObject->onConfiguration(config);
            config.map.addChild(newObject);

            if (!nameAssigner.count(name)) {
                return;
            }

            for (auto& callback : nameAssigner.at(name)) {
                callback(*newObject);
            }
        });
    }
};

} // namespace io
} // namespace avalon

#endif /* AVALON_IO_TILEDMAPLOADER_H */