#include "TiledMapLoader.h"

#include "cocos2d.h"

using namespace cocos2d;

namespace avalon {
namespace io {

TiledMapLoader::TiledMapLoader(const std::string& mapFileName)
: mapFileName(mapFileName)
{
}

void TiledMapLoader::setBox2dContainer(avalon::physics::Box2dContainer& container)
{
    box2dContainer = &container;
}

bool TiledMapLoader::isFiltered(const std::string& name, const std::list<std::string>& filter)
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

cocos2d::TMXTiledMap* TiledMapLoader::load()
{
    auto map = TMXTiledMap::create(mapFileName.c_str());
    if (!map) {
        throw std::invalid_argument("Mapfile not found!");
    }

    loadGidFactories(*map);
    loadNamedFactories(*map);

    return map;
}

void TiledMapLoader::loadGidFactories(cocos2d::TMXTiledMap& map)
{
    Value* info = nullptr;

    for (auto& child : map.getChildren()) {
        auto mapLayer = dynamic_cast<TMXLayer*>(child);
        if (!mapLayer) {
            continue;
        }

        for (int x = 0; x < map.getMapSize().width; ++x) {
            for (int y = 0; y < map.getMapSize().height; ++y) {

                auto currentGID = mapLayer->getTileGIDAt({static_cast<float>(x), static_cast<float>(y)});
                if (!gidFactories.count(currentGID)) {
                    continue;
                }

                map.getPropertiesForGID(currentGID, &info);
                auto data = info->asValueMap();

                if (!data.count("gid")) data["gid"] = Value(static_cast<int>(currentGID));
                if (!data.count("x")) data["x"] = Value(static_cast<float>(x));
                if (!data.count("y")) data["y"] = Value(static_cast<float>(y));

                Configuration config{data, mapLayer->getLayerName(), map, box2dContainer};
                for (auto& callback : gidFactories.at(currentGID)) {
                    callback(config);
                }
            }
        }
    }
}

void TiledMapLoader::loadNamedFactories(cocos2d::TMXTiledMap& map)
{
    for (auto& arrayElement : map.getObjectGroups()) {
        auto objectGroup = dynamic_cast<TMXObjectGroup*>(arrayElement);
        if (!objectGroup) {
            continue;
        }

        for (auto& value : objectGroup->getObjects()) {
            auto valueMap = value.asValueMap();
            if (valueMap.empty())
                continue;

            if (!valueMap.count("name")) {
                continue;
            }

            auto name = valueMap["name"].asString();
            if (!nameFactories.count(name)) {
                continue;
            }

            Configuration config{valueMap, objectGroup->getGroupName(), map, box2dContainer};
            for (auto& callback : nameFactories.at(name)) {
                callback(config);
            }
        }
    }
}

void TiledMapLoader::registerCallbackForName(const std::string& name, const Callback& callback, const std::list<std::string>& layerFilter)
{
    if (!nameFactories.count(name)) {
        nameFactories[name] = CallbackList();
    }

    nameFactories[name].push_back([this, callback, layerFilter](const Configuration& config) {
        if (!isFiltered(config.layer, layerFilter)) {
            callback(config);
        }
    });
}

void TiledMapLoader::registerCallbackForGID(const int gid, const Callback& callback, const std::list<std::string>& layerFilter)
{
    if (!gidFactories.count(gid)) {
        gidFactories[gid] = CallbackList();
    }

    gidFactories[gid].push_back([this, callback, layerFilter](const Configuration& config) {
        if (!isFiltered(config.layer, layerFilter)) {
            callback(config);
        }
    });
}

} // namespace io
} // namespace avalon
