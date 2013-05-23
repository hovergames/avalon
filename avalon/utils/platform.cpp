#include <avalon/utils/platform.h>

namespace avalon {
namespace utils {
namespace platform {

std::string getName()
{
#if   AVALON_PLATFORM_IS_IOS
    return std::string("ios");
#elif AVALON_PLATFORM_IS_ANDROID
    return std::string("android");
#elif AVALON_PLATFORM_IS_WIN32
    return std::string("win32");
#elif AVALON_PLATFORM_IS_MARMALADE
    return std::string("marmalade");
#elif AVALON_PLATFORM_IS_LINUX
    return std::string("linux");
#elif AVALON_PLATFORM_IS_BADA
    return std::string("bada");
#elif AVALON_PLATFORM_IS_BLACKBERRY
    return std::string("blackberry");
#elif AVALON_PLATFORM_IS_MAC
    return std::string("mac");
#elif AVALON_PLATFORM_IS_NACL
    return std::string("nacl");
#else
    #error "No name defined for current used CC_TARGET_PLATFORM"
#endif
}

std::string getFlavor()
{
#if   AVALON_PLATFORM_IS_ANDROID_AMAZON
    return std::string("amazon");
#elif AVALON_PLATFORM_IS_ANDROID_GOOGLE
    return std::string("google");
#elif AVALON_PLATFORM_IS_ANDROID_SAMSUNG
    return std::string("samsung");
#else
    return std::string();
#endif
}

} // namespace platform
} // namespace utils
} // namespace avalon
