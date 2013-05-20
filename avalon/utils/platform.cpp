#include <avalon/utils/platform.h>

namespace avalon {
namespace utils {
namespace platform {

std::string getName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return std::string("ios");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return std::string("android");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    return std::string("win32");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
    return std::string("marmalade");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    return std::string("linux");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_BADA)
    return std::string("bada");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
    return std::string("blackberry");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    return std::string("mac");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_NACL)
    return std::string("nacl");
#else
    #error "No name defined for current used CC_TARGET_PLATFORM"
#endif
}

std::string getFlavor()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    return std::string();
#else
    #if (AVALON_PLATFORM_FLAVOR == AVALON_PLATFORM_FLAVOR_AMAZON)
        return std::string("amazon");
    #elif (AVALON_PLATFORM_FLAVOR == AVALON_PLATFORM_FLAVOR_GOOGLE)
        return std::string("google");
    #elif (AVALON_PLATFORM_FLAVOR == AVALON_PLATFORM_FLAVOR_SAMSUNG)
        return std::string("samsung");
    #else
        #error "No name defined for current used AVALON_PLATFORM_FLAVOR"
    #endif
#endif
}

} // namespace platform
} // namespace utils
} // namespace avalon
