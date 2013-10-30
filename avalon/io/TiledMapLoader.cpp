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

std::shared_ptr<cocos2d::TMXTiledMap> TiledMapLoader::load()
{
    auto map = std::shared_ptr<TMXTiledMap>(TMXTiledMap::create(mapFileName.c_str()));
    if (!map) throw std::invalid_argument("Mapfile not found!");

    for (auto& child : *map->getChildren()) {
        auto mapLayer = dynamic_cast<TMXLayer*>(child);
        if (!mapLayer) continue;

        for (int x = 0; x < mapLayer->getMapTileSize().width; ++x) {
            for (int y = 0; y < mapLayer->getMapTileSize().height; ++y) {
                auto currentGID = mapLayer->getTileGIDAt({x, y});

                for (auto& obj : gidFactories) {
                    if (obj.first != currentGID) continue;

                    auto info = map->getPropertiesForGID(currentGID);
                    auto data = avalon::utils::cocos::to_map<std::string>(*info);

                    obj.second(*map.get(), mapLayer->getLayerName(), data);
                }
            }
        }
    }

    // ObjectGroups
    for (auto& arrayElement : *map->getObjectGroups()) {
        auto objectGroup = dynamic_cast<TMXObjectGroup*>(arrayElement);
        if (!objectGroup) continue;

        for (auto& arrayElement : *objectGroup->getObjects()) {
            auto objectDictonary = dynamic_cast<Dictionary*>(arrayElement);
            if (!objectDictonary) continue;

            auto data = avalon::utils::cocos::to_map<std::string>(*objectDictonary);
            if (!data.count("name")) continue;

            for (auto& obj : nameFactories) {
                if (obj.first != data["name"]) continue;

                obj.second(*map.get(), objectGroup->getGroupName(), data);
            }
        }
    }

    return map;
}

} // namespace io
} // namespace avalon