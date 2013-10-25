#ifdef AVALON_CONFIG_ADS_PROVIDER_SAMSUNGADHUB_ENABLED

#include <avalon/ads/provider/SamsungAdHub.h>

#include <jni.h>
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <boost/assert.hpp>

namespace avalon {
namespace ads {
namespace provider {

/**
 * C++ -->> Java
 */

namespace samsungadhub {
namespace helper {

const char* const CLASS_NAME = "com/avalon/ads/SamsungAdHubBridge";

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

} // namespace helper
} // namespace samsungadhub

/**
 * Public API
 */

SamsungAdHub::SamsungAdHub()
: inventoryId()
{
}

void SamsungAdHub::init()
{
    BOOST_ASSERT_MSG(inventoryId != "", "inventoryId must be set first");
    samsungadhub::helper::callStaticVoidMethodWithString("init", inventoryId.c_str());
}

void SamsungAdHub::hideAds()
{
    samsungadhub::helper::callStaticVoidMethod("hideAds");
}

void SamsungAdHub::showFullscreenAd()
{
    samsungadhub::helper::callStaticVoidMethod("showFullscreenAd");
}

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_CONFIG_ADS_PROVIDER_SAMSUNGADHUB_ENABLED */