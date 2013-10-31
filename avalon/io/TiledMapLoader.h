#ifndef AVALON_IO_TILEDMAPLOADER_H
#define AVALON_IO_TILEDMAPLOADER_H

#include "cocos2d.h"

namespace avalon {
namespace io {

class TiledMapLoader
{
public:
    typedef std::map<std::string, std::string> Dictonary;
    typedef std::function<void(cocos2d::TMXTiledMap&, const std::string&, const Dictonary&)> Callback;

private:
    std::map<int, Callback> gidFactories;
    std::map<std::string, Callback> nameFactories;

    const std::string mapFileName;

    bool isFiltered(const std::string& name, const std::list<std::string> filter = {});
    void loadGidFactories(cocos2d::TMXTiledMap& map);
    void loadNamedFactories(cocos2d::TMXTiledMap& map);
    
public:
    TiledMapLoader(const std::string mapFileName);
    std::shared_ptr<cocos2d::TMXTiledMap> load();
    void registerCallbackForName(const std::string& name, const Callback& callback, const std::list<std::string> layerFilter = {});

    template<typename T>
    void registerObjectForGID(const int gid, const std::list<std::string> layerFilter = {})
    {
        gidFactories[gid] = [this, layerFilter](cocos2d::TMXTiledMap& map, const std::string& layerName, const Dictonary& data)
        {
            if (!isFiltered(layerName, layerFilter)) {
                auto newObject = T::create();
                newObject->onTiledConfiguration(map, layerName, data);
                map.addChild(newObject);
            }
        };
    }

    template<typename T>
    void registerObjectForName(const std::string& name, const std::list<std::string> layerFilter = {})
    {
        nameFactories[name] = [this, layerFilter](cocos2d::TMXTiledMap& map, const std::string& layerName, const Dictonary& data)
        {
            if (!isFiltered(layerName, layerFilter)) {
                auto newObject = T::create();
                newObject->onTiledConfiguration(map, layerName, data);
                map.addChild(newObject);
            }
        };
    }
};


} // namespace io
} // namespace avalon

#endif /* AVALON_IO_TILEDMAPLOADER_H */