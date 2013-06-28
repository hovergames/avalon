#include <avalon/ads/provider/TapForTap.h>

#include <jni.h>
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <boost/assert.hpp>

namespace avalon {
namespace ads {
namespace provider {

namespace helper {
namespace tapfortap {

const char* const CLASS_NAME = "com/avalon/ads/TapForTapBridge";

/**
 * C++ -->> Java
 */

void callStaticVoidMethod(const char* name)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void callStaticVoidMethodWithString(const char* name, const char* argument)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "(Ljava/lang/String;)V")) {
        jstring jProductId = t.env->NewStringUTF(argument);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jProductId);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jProductId);
    }
}

} // namespace tapfortap
} // namespace helper

/**
 * Public API
 */

TapForTap::TapForTap()
: apiKey()
{
}

void TapForTap::init()
{
    BOOST_ASSERT_MSG(apiKey != "", "apiKey must be set first");
    helper::tapfortap::callStaticVoidMethodWithString("init", apiKey.c_str());
}

void TapForTap::hideAds()
{
    helper::tapfortap::callStaticVoidMethod("hideAds");
}

void TapForTap::showFullscreenAd()
{
    helper::tapfortap::callStaticVoidMethod("showFullscreenAd");
}

void TapForTap::showBanner()
{
    helper::tapfortap::callStaticVoidMethod("showBanner");
}

} // namespace provider
} // namespace ads
} // namespace avalon