#include <avalon/utils/platform.h>

#if !AVALON_PLATFORM_IS_ANDROID
    #error "This file is only for the Android platform"
#endif

#if AVALON_PLATFORM_IS_ANDROID_AMAZON || defined(AVALON_CONFIG_GAMECENTER_USE_AMAZON_ON_GOOGLE)
    #include <avalon/platform/android-amazon/gamecenter/GameCenter.cpp>
#else
    #include <avalon/platform/android-google/gamecenter/GameCenter.cpp>
#endif