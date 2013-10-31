#include "TiledMapLoader.h"

#include "cocos2d.h"
#include <avalon/utils/cocos.h>

using namespace cocos2d;

namespace avalon {
namespace io {

TiledMapLoader::TiledMapLoader(std::string mapFileName)
: mapFileName(mapFileName)
{
}

bool TiledMapLoader::isFiltered(const std::string& name, const std::list<std::string> filter)
{
    if (filter.empty()) {
        return false;
    }

    auto iter = std::find(filter.begin(), filter.end(), name);
    if (iter == filter.end()) {
        return true;
    }

    return false;
}

std::shared_ptr<cocos2d::TMXTiledMap> TiledMapLoader::load()
{
    auto map = std::shared_ptr<TMXTiledMap>(TMXTiledMap::create(mapFileName.c_str()));
    if (!map) {
        throw std::invalid_argument("Mapfile not found!");
    }

    loadGidFactories(*map.get());
    loadNamedFactories(*map.get());

    return map;
}

void TiledMapLoader::loadGidFactories(cocos2d::TMXTiledMap& map)
{
    for (auto& child : *map.getChildren()) {
        auto mapLayer = dynamic_cast<TMXLayer*>(child);
        if (!mapLayer) {
            continue;
        }

        for (int x = 0; x < mapLayer->getMapTileSize().width; ++x) {
            for (int y = 0; y < mapLayer->getMapTileSize().height; ++y) {
                auto currentGID = mapLayer->getTileGIDAt({x, y});
                auto info = map.getPropertiesForGID(currentGID);
                auto data = avalon::utils::cocos::to_map<std::string>(*info);

                for (auto& obj : gidFactories) {
                    if (obj.first == currentGID) {
                        obj.second(map, mapLayer->getLayerName(), data);
                    }
                }
            }
        }
    }
}

void TiledMapLoader::loadNamedFactories(cocos2d::TMXTiledMap& map)
{
    for (auto& arrayElement : *map.getObjectGroups()) {
        auto objectGroup = dynamic_cast<TMXObjectGroup*>(arrayElement);
        if (!objectGroup) {
            continue;
        }

        for (auto& arrayElement : *objectGroup->getObjects()) {
            auto objectDictonary = dynamic_cast<Dictionary*>(arrayElement);
            if (!objectDictonary) {
                continue;
            }

            auto data = avalon::utils::cocos::to_map<std::string>(*objectDictonary);
            if (!data.count("name")) {
                continue;
            }

            for (auto& obj : nameFactories) {
                if (obj.first != data["name"]) {
                    continue;
                }

                obj.second(map, objectGroup->getGroupName(), data);
            }
        }
    }
}

void TiledMapLoader::registerCallbackForName(const std::string& name, const Callback& callback, const std::list<std::string> layerFilter)
{
    nameFactories[name] = [this, layerFilter, callback](cocos2d::TMXTiledMap& map, const std::string& layerName, const Dictonary& data)
    {
        if (!isFiltered(layerName, layerFilter)) {
            callback(map, layerName, data);
        }
    };
}

} // namespace io
} // namespace avalon