#include <avalon/utils/conversion.h>

namespace avalon {
namespace utils {
namespace conversion {

boost::any toAny(cocos2d::Object* object)
{
    if (!object) {
        return nullptr;
    } else if (auto value = dynamic_cast<cocos2d::String*>(object)) {
        return std::string(value->getCString());
    } else if (auto value = dynamic_cast<cocos2d::Integer*>(object)) {
        return value->getValue();
    } else if (auto value = dynamic_cast<cocos2d::Float*>(object)) {
        return value->getValue();
    } else if (auto value = dynamic_cast<cocos2d::Double*>(object)) {
        return value->getValue();
    } else if (auto value = dynamic_cast<cocos2d::Bool*>(object)) {
        return value->getValue();
    } else if (auto value = dynamic_cast<cocos2d::Dictionary*>(object)) {
        // We're unable to detect the right key type WITHOUT messing
        // around with UT_HASH and .. well .. std::string is good enough here :)
        return toMap<std::string>(*value);
    } else if (auto value = dynamic_cast<cocos2d::Array*>(object)) {
        return toList(*value);
    } else if (auto value = dynamic_cast<cocos2d::Set*>(object)) {
        return toList(*value);
    } else {
        throw new std::runtime_error("Unknown type");
    }
}

std::list<boost::any> toList(cocos2d::Array& array)
{
    std::list<boost::any> data;
    std::for_each(
        array.begin(), array.end(),
        [&data](cocos2d::Object* obj) { data.push_back(toAny(obj)); }
    );
    return data;
}

std::list<boost::any> toList(cocos2d::Set& set)
{
    std::list<boost::any> data;
    std::for_each(
        set.begin(), set.end(),
        [&data](cocos2d::Object* obj) { data.push_back(toAny(obj)); }
    );
    return data;
}

} // namespace conversion
} // namespace utils
} // namespace avalon
