#include "TiledMapLoader.h"

#include "cocos2d.h"
#include <avalon/utils/cocos.h>

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

void TiledMapLoader::load(cocos2d::TMXTiledMap** map)
{
    *map = TMXTiledMap::create(mapFileName.c_str());
    if (!*map) {
        throw std::invalid_argument("Mapfile not found!");
    }

    loadGidFactories(**map);
    loadNamedFactories(**map);
}

std::list<cocos2d::Point> TiledMapLoader::convertToPointList(const boost::any& original)
{
    using AnyList = std::list<boost::any>;
    using AnyMap = std::map<std::string, boost::any>;

    std::list<cocos2d::Point> result;
    auto list = boost::any_cast<AnyList>(original);

    for (auto& record : list) {
        auto values = boost::any_cast<AnyMap>(record);
        auto x = std::stof(boost::any_cast<std::string>(values["x"]));
        auto y = std::stof(boost::any_cast<std::string>(values["y"]));
        result.push_back(Point(x, y));
    }

    return result;
}

boost::any TiledMapLoader::convertToFloat(boost::any& value)
{
    if (value.type() == typeid(float)) {
        return value;
    } else {
        auto valueString = boost::any_cast<std::string>(value);
        if (valueString.empty()) {
            return boost::any(0.0f);
        } else {
            auto valueFloat = std::stof(valueString);
            return boost::any(valueFloat);
        }
    }
}

void TiledMapLoader::loadGidFactories(cocos2d::TMXTiledMap& map)
{
    for (auto& child : *map.getChildren()) {
        auto mapLayer = dynamic_cast<TMXLayer*>(child);
        if (!mapLayer) {
            continue;
        }

        for (int x = 0; x < map.getMapSize().width; ++x) {
            for (int y = 0; y < map.getMapSize().height; ++y) {
                auto currentGID = mapLayer->getTileGIDAt({x, y});
                auto info = map.getPropertiesForGID(currentGID);
                auto data = avalon::utils::cocos::to_map<std::string>(*info);

                if (!data.count("gid")) data["gid"] = currentGID;
                if (!data.count("x")) data["x"] = static_cast<float>(x);
                if (!data.count("y")) data["y"] = static_cast<float>(y);
                data["x"] = convertToFloat(data["x"]);
                data["y"] = convertToFloat(data["y"]);

                for (auto& obj : gidFactories) {
                    if (obj.first == currentGID) {
                        Configuration config{data, mapLayer->getLayerName(), map, box2dContainer};
                        obj.second(config);
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
            auto objectDictonary = dynamic_cast<cocos2d::Dictionary*>(arrayElement);
            if (!objectDictonary) {
                continue;
            }

            auto data = avalon::utils::cocos::to_map<std::string>(*objectDictonary);
            if (!data.count("name")) {
                continue;
            }

            if (data.count("x")) data["x"] = convertToFloat(data["x"]);
            if (data.count("y")) data["y"] = convertToFloat(data["y"]);
            if (data.count("width")) data["width"] = convertToFloat(data["width"]);
            if (data.count("height")) data["height"] = convertToFloat(data["height"]);
            if (data.count("points")) data["points"] = convertToPointList(data["points"]);
            if (data.count("polylinePoints")) data["polylinePoints"] = convertToPointList(data["polylinePoints"]);

            for (auto& obj : nameFactories) {
                auto dataName = boost::any_cast<std::string>(data["name"]);
                if (obj.first != dataName) {
                    continue;
                }

                Configuration config{data, objectGroup->getGroupName(), map, box2dContainer};
                obj.second(config);
            }
        }
    }
}

void TiledMapLoader::registerCallbackForName(const std::string& name, const Callback& callback, const std::list<std::string>& layerFilter)
{
    nameFactories[name] = [this, layerFilter, callback](const Configuration& config)
    {
        if (!isFiltered(config.layer, layerFilter)) {
            callback(config);
        }
    };
}

} // namespace io
} // namespace avalon