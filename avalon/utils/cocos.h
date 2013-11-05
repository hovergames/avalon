#ifndef AVALON_UTILS_COCOS2D_H
#define AVALON_UTILS_COCOS2D_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include <boost/any.hpp>

namespace avalon {
namespace utils {
namespace cocos {

bool operator==(const cocos2d::Color3B& a, const cocos2d::Color3B& b);
bool operator==(const cocos2d::Color3B& a, const cocos2d::Color4B& b);
bool operator==(const cocos2d::Color3B& a, const cocos2d::Color4F& b);
bool operator==(const cocos2d::Color3B& a, const cocos2d::extension::HSV& b);
bool operator==(const cocos2d::Color4B& a, const cocos2d::Color3B& b);
bool operator==(const cocos2d::Color4B& a, const cocos2d::Color4B& b);
bool operator==(const cocos2d::Color4B& a, const cocos2d::Color4F& b);
bool operator==(const cocos2d::Color4B& a, const cocos2d::extension::HSV& b);
bool operator==(const cocos2d::Color4F& a, const cocos2d::Color3B& b);
bool operator==(const cocos2d::Color4F& a, const cocos2d::Color4B& b);
bool operator==(const cocos2d::Color4F& a, const cocos2d::Color4F& b);
bool operator==(const cocos2d::Color4F& a, const cocos2d::extension::HSV& b);
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::Color3B& b);
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::Color4B& b);
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::Color4F& b);
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::extension::HSV& b);
bool operator!=(const cocos2d::Color3B& a, const cocos2d::Color3B& b);
bool operator!=(const cocos2d::Color3B& a, const cocos2d::Color4B& b);
bool operator!=(const cocos2d::Color3B& a, const cocos2d::Color4F& b);
bool operator!=(const cocos2d::Color3B& a, const cocos2d::extension::HSV& b);
bool operator!=(const cocos2d::Color4B& a, const cocos2d::Color3B& b);
bool operator!=(const cocos2d::Color4B& a, const cocos2d::Color4B& b);
bool operator!=(const cocos2d::Color4B& a, const cocos2d::Color4F& b);
bool operator!=(const cocos2d::Color4B& a, const cocos2d::extension::HSV& b);
bool operator!=(const cocos2d::Color4F& a, const cocos2d::Color3B& b);
bool operator!=(const cocos2d::Color4F& a, const cocos2d::Color4B& b);
bool operator!=(const cocos2d::Color4F& a, const cocos2d::Color4F& b);
bool operator!=(const cocos2d::Color4F& a, const cocos2d::extension::HSV& b);
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::Color3B& b);
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::Color4B& b);
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::Color4F& b);
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::extension::HSV& b);

cocos2d::extension::HSV toHSV(const cocos2d::Color3B& color);
cocos2d::extension::HSV toHSV(const cocos2d::Color4B& color);
cocos2d::extension::HSV toHSV(const cocos2d::Color4F& color);
cocos2d::Color3B toRGB(const cocos2d::Color4B& color);
cocos2d::Color3B toRGB(const cocos2d::Color4F& color);
cocos2d::Color3B toRGB(const cocos2d::extension::HSV& color);
cocos2d::Color4F toRGBA(const cocos2d::Color3B& color);
cocos2d::Color4F toRGBA(const cocos2d::Color4B& color);
cocos2d::Color4B toRGBA(const cocos2d::Color4F& color);
cocos2d::Color4F toRGBA(const cocos2d::extension::HSV& color);

boost::any toAny(cocos2d::Object* object);
std::list<boost::any> toList(cocos2d::Array& array);
std::list<boost::any> toList(cocos2d::Set& set);

template<typename Key>
std::map<Key, boost::any> toMap(cocos2d::Dictionary& dict)
{
    // The following lines may look weird but CCDICT_FOREACH is
    // a little bit broken ...
    using namespace cocos2d;
    const cocos2d::Dictionary* dictPtr = &dict;

    std::map<Key, boost::any> data;
    cocos2d::DictElement* dictElement = nullptr;

    CCDICT_FOREACH(dictPtr, dictElement) {
        Key key;
        if (std::is_same<Key, std::string>::value) {
            key = std::string(dictElement->getStrKey());
        } else {
            key = dictElement->getIntKey();
        }

        data[key] = toAny(dictElement->getObject());
    }

    return data;
}

} // namespace cocos
} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_COCOS2D_H */
