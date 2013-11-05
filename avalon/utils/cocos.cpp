#include <avalon/utils/cocos.h>

namespace avalon {
namespace utils {
namespace cocos {

bool isSameColor(const cocos2d::Color3B& a, const cocos2d::Color3B& b)
{
    return (a.r == b.r && a.g == b.g && a.b == b.b);
}

bool isSameColor(const cocos2d::Color4B& a, const cocos2d::Color4B& b)
{
    return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

bool isSameColor(const cocos2d::Color4F& a, const cocos2d::Color4F& b)
{
    return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

boost::any to_any(cocos2d::Object* object)
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
        return to_map<std::string>(*value);
    } else if (auto value = dynamic_cast<cocos2d::Array*>(object)) {
        return to_list(*value);
    } else if (auto value = dynamic_cast<cocos2d::Set*>(object)) {
        return to_list(*value);
    } else {
        throw new std::runtime_error("Unknown type");
    }
}

std::list<boost::any> to_list(cocos2d::Array& array)
{
    std::list<boost::any> data;
    std::for_each(
        array.begin(), array.end(),
        [&data](cocos2d::Object* obj) { data.push_back(to_any(obj)); }
    );
    return data;
}

std::list<boost::any> to_list(cocos2d::Set& set)
{
    std::list<boost::any> data;
    std::for_each(
        set.begin(), set.end(),
        [&data](cocos2d::Object* obj) { data.push_back(to_any(obj)); }
    );
    return data;
}

} // namespace cocos
} // namespace utils
} // namespace avalon
