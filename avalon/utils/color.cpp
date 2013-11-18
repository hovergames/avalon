#include <avalon/utils/color.h>

using namespace cocos2d::extension;

namespace avalon {
namespace utils {
namespace color {

bool operator==(const cocos2d::extension::HSV& a, const cocos2d::extension::HSV& b)
{
    return (a.h == b.h && a.s == b.s && a.v == b.v);
}

bool operator==(const cocos2d::Color3B& a, const cocos2d::extension::HSV& b)        { return (a == toRGB (b)); }
bool operator==(const cocos2d::Color4B& a, const cocos2d::extension::HSV& b)        { return (a == toRGBA(b)); }
bool operator==(const cocos2d::Color4F& a, const cocos2d::extension::HSV& b)        { return (a == toRGBA(b)); }
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::Color3B& b)        { return (a == toHSV(b)); }
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::Color4B& b)        { return (a == toHSV(b)); }
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::Color4F& b)        { return (a == toHSV(b)); }
bool operator!=(const cocos2d::Color3B& a, const cocos2d::extension::HSV& b)        { return !(a == b); }
bool operator!=(const cocos2d::Color4B& a, const cocos2d::extension::HSV& b)        { return !(a == b); }
bool operator!=(const cocos2d::Color4F& a, const cocos2d::extension::HSV& b)        { return !(a == b); }
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::Color3B& b)        { return !(a == b); }
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::Color4B& b)        { return !(a == b); }
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::Color4F& b)        { return !(a == b); }
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::extension::HSV& b) { return !(a == b); }

cocos2d::extension::HSV toHSV(const cocos2d::Color3B& color)
{
    return ControlUtils::HSVfromRGB({color.r / 255.0, color.g / 255.0, color.b / 255.0, 1.0});
}

cocos2d::extension::HSV toHSV(const cocos2d::Color4B& color)
{
    return ControlUtils::HSVfromRGB({color.r / 255.0, color.g / 255.0, color.b / 255.0, color.a / 255.0});
}

cocos2d::extension::HSV toHSV(const cocos2d::Color4F& color)
{
    return ControlUtils::HSVfromRGB({color.r, color.g, color.b, color.a});
}

cocos2d::Color3B toRGB(const cocos2d::extension::HSV& color)
{
    auto rgba = ControlUtils::RGBfromHSV(color);
    return {rgba.r * 255, rgba.g * 255, rgba.b * 255};
}

cocos2d::Color4F toRGBA(const cocos2d::extension::HSV& color)
{
    auto rgba = ControlUtils::RGBfromHSV(color);
    return {rgba.r, rgba.g, rgba.b, rgba.a};
}

} // namespace color
} // namespace utils
} // namespace avalon
