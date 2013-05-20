#ifndef AVALON_UTILS_PLATFORM_H
#define AVALON_UTILS_PLATFORM_H

#include <string>
#include "cocos2d.h"

#define AVALON_PLATFORM_FLAVOR_AMAZON 1
#define AVALON_PLATFORM_FLAVOR_GOOGLE 2
#define AVALON_PLATFORM_FLAVOR_SAMSUNG 3

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #ifndef AVALON_PLATFORM_FLAVOR
        #error "You have to define AVALON_PLATFORM_FLAVOR first"
    #elif (AVALON_PLATFORM_FLAVOR != AVALON_PLATFORM_FLAVOR_AMAZON) \
       && (AVALON_PLATFORM_FLAVOR != AVALON_PLATFORM_FLAVOR_GOOGLE) \
       && (AVALON_PLATFORM_FLAVOR != AVALON_PLATFORM_FLAVOR_SAMSUNG)
        #error "Invalid AVALON_PLATFORM_FLAVOR defined"
    #endif
#else
    #define AVALON_PLATFORM_FLAVOR 0
#endif

namespace avalon {
namespace utils {
namespace platform {

std::string getName();
std::string getFlavor();

} // namespace platform
} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_PLATFORM_H */
