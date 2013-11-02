#ifndef AVALON_IO_TILEDMAPLOADER_H
#define AVALON_IO_TILEDMAPLOADER_H

#include "cocos2d.h"
#include <boost/any.hpp>

namespace avalon { namespace physics { class Box2dContainer; } }

namespace avalon {
namespace io {

class TiledMapLoader
{
public:
    using Dictionary = std::map<std::string, boost::any>;
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

    std::unordered_map<int, Callback> gidFactories;
    std::unordered_map<std::string, Callback> nameFactories;
    std::unordered_map<int, Assigner> gidAssigner;
    std::unordered_map<std::string, Assigner> nameAssigner;
    avalon::physics::Box2dContainer* box2dContainer = nullptr;
    const std::string mapFileName;

    boost::any convertToFloat(boost::any& value);
    bool isFiltered(const std::string& name, const std::list<std::string>& filter = {});
    void loadGidFactories(cocos2d::TMXTiledMap& map);
    void loadNamedFactories(cocos2d::TMXTiledMap& map);
    
public:
    explicit TiledMapLoader(const std::string& mapFileName);
    std::shared_ptr<cocos2d::TMXTiledMap> load();
    void registerCallbackForName(const std::string& name, const Callback& callback, const std::list<std::string>& layerFilter = {});
    void setBox2dContainer(avalon::physics::Box2dContainer& container);

    template<typename T>
    void assignObjectsByGID(const int gid, std::list<T*>& list)
    {
        if (!gidFactories.count(gid)) {
            throw new std::runtime_error("GID not registered yet");
        }

        gidAssigner[gid] = [&list](cocos2d::Node& node) {
            auto target = dynamic_cast<T*>(&node);
            if (!target) {
                throw new std::invalid_argument("Wrong Object type");
            }
            list.push_back(target);
        };
    }

    template<typename T>
    void assignObjectsByName(const std::string& name, std::list<T*>& list)
    {
        if (!nameFactories.count(name)) {
            throw new std::runtime_error("Name not registered yet");
        }

        nameAssigner[name] = [&list](cocos2d::Node& node) {
            auto target = dynamic_cast<T*>(&node);
            if (!target) {
                throw new std::invalid_argument("Wrong Object type");
            }
            list.push_back(target);
        };
    }

    template<typename T>
    void registerObjectForGID(const int gid, const std::list<std::string>& layerFilter = {})
    {
        if (gidFactories.count(gid)) {
            throw new std::runtime_error("GID already registered");
        }

        gidFactories[gid] = [this, gid, layerFilter](const Configuration& config)
        {
            if (!isFiltered(config.layer, layerFilter)) {
                auto newObject = T::create();
                newObject->onConfiguration(config);
                config.map.addChild(newObject);

                if (gidAssigner.count(gid)) {
                    gidAssigner[gid](*newObject);
                }
            }
        };
    }

    template<typename T>
    void registerObjectForName(const std::string& name, const std::list<std::string>& layerFilter = {})
    {
        if (nameFactories.count(name)) {
            throw new std::runtime_error("Name already registered");
        }

        nameFactories[name] = [this, name, layerFilter](const Configuration& config)
        {
            if (!isFiltered(config.layer, layerFilter)) {
                auto newObject = T::create();
                newObject->onConfiguration(config);
                config.map.addChild(newObject);

                if (nameAssigner.count(name)) {
                    nameAssigner[name](*newObject);
                }
            }
        };
    }
};

} // namespace io
} // namespace avalon

#endif /* AVALON_IO_TILEDMAPLOADER_H */