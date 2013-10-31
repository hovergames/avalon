#ifndef AVALON_UTILS_COCOS2D_H
#define AVALON_UTILS_COCOS2D_H

#include "cocos2d.h"

namespace avalon {
namespace utils {
namespace cocos {

template<typename V>
std::unordered_map<std::string, V> to_unordered_map(const cocos2d::Dictionary& dict)
{
    using namespace cocos2d;

    std::unordered_map<std::string, V> data;
    cocos2d::DictElement* dictElement = nullptr;

    const cocos2d::Dictionary* pDict = &dict;
    CCDICT_FOREACH(pDict, dictElement)
    {
        std::string key(dictElement->getStrKey());
        std::string value = dynamic_cast<cocos2d::String*>(dictElement->getObject())->getCString();
        data[key] = value;
    }

    return data;
}

} // namespace cocos
} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_COCOS2D_H */
