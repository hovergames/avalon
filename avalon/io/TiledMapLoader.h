#ifndef AVALON_IO_TILEDMAPLOADER_H
#define AVALON_IO_TILEDMAPLOADER_H

#include "cocos2d.h"

namespace avalon {
namespace io {

class TiledMapLoader
{
public:
    typedef std::map<std::string, std::string> Dictonary;
    typedef std::function<void(cocos2d::TMXTiledMap&, const std::string&, const Dictonary&)> Factory;

    const std::string mapFileName;

    std::map<int, Factory> gidFactories;
    std::map<std::string, Factory> nameFactories;


    TiledMapLoader(const std::string mapFileName);

    std::shared_ptr<cocos2d::TMXTiledMap> load();

    template<typename T>
    void registerObjectForGID(const int gid, const std::list<std::string> layerFilter = {})
    {
        gidFactories[gid] = [layerFilter](cocos2d::TMXTiledMap& map, const std::string& layerName, const Dictonary& data) {
            if (!layerFilter.empty()) {
                auto iter = std::find(layerFilter.begin(), layerFilter.end(), layerName);
                if (iter == layerFilter.end()) {
                    return;
                }
            }

            auto newObject = T::create();
            newObject->onTiledConfiguration(map, layerName, data);
            map.addChild(newObject);
        };
    }

    template<typename T>
    void registerObjectForName(const std::string& name, const std::list<std::string> layerFilter = {})
    {
        nameFactories[name] = [layerFilter](cocos2d::TMXTiledMap& map, const std::string& layerName, const Dictonary& data) {
            if (!layerFilter.empty()) {
                auto iter = std::find(layerFilter.begin(), layerFilter.end(), layerName);
                if (iter == layerFilter.end()) {
                    return;
                }
            }

            auto newObject = T::create();
            newObject->onTiledConfiguration(map, layerName, data);
            map.addChild(newObject);
        };
    }
};

} // namespace io
} // namespace avalon

#endif /* AVALON_IO_TILEDMAPLOADER_H */