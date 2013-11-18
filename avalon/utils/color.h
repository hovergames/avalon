#ifndef AVALON_UTILS_COLOR_H
#define AVALON_UTILS_COLOR_H

#include "cocos2d.h"
#include "cocos-ext.h"

namespace avalon {
namespace utils {
namespace color {

bool operator==(const cocos2d::Color3B& a, const cocos2d::extension::HSV& b);
bool operator==(const cocos2d::Color4B& a, const cocos2d::extension::HSV& b);
bool operator==(const cocos2d::Color4F& a, const cocos2d::extension::HSV& b);
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::Color3B& b);
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::Color4B& b);
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::Color4F& b);
bool operator==(const cocos2d::extension::HSV& a, const cocos2d::extension::HSV& b);
bool operator!=(const cocos2d::Color3B& a, const cocos2d::extension::HSV& b);
bool operator!=(const cocos2d::Color4B& a, const cocos2d::extension::HSV& b);
bool operator!=(const cocos2d::Color4F& a, const cocos2d::extension::HSV& b);
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::Color3B& b);
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::Color4B& b);
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::Color4F& b);
bool operator!=(const cocos2d::extension::HSV& a, const cocos2d::extension::HSV& b);

cocos2d::extension::HSV toHSV(const cocos2d::Color3B& color);
cocos2d::extension::HSV toHSV(const cocos2d::Color4B& color);
cocos2d::extension::HSV toHSV(const cocos2d::Color4F& color);
cocos2d::Color3B toRGB(const cocos2d::extension::HSV& color);
cocos2d::Color4F toRGBA(const cocos2d::extension::HSV& color);

} // namespace color
} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_COLOR_H */
