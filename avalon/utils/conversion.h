#ifndef AVALON_UTILS_CONVERSION_H
#define AVALON_UTILS_CONVERSION_H

#include "cocos2d.h"
#include <boost/any.hpp>

namespace avalon {
namespace utils {
namespace conversion {

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

} // namespace conversion
} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_CONVERSION_H */
