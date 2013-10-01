#ifdef AVALON_CONFIG_ADS_PROVIDER_REVMOB_ENABLED

#include <avalon/ads/provider/Revmob.h>

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

namespace revmob {
namespace helper {

const char* const CLASS_NAME = "com/avalon/ads/RevmobBridge";

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
} // namespace revmob

/**
 * Public API
 */

Revmob::Revmob()
: appId()
{
}

void Revmob::init()
{
    BOOST_ASSERT_MSG(appId != "", "appId must be set first");
    revmob::helper::callStaticVoidMethodWithString("init", appId.c_str());
}

void Revmob::hideAds()
{
    revmob::helper::callStaticVoidMethod("hideAds");
}

void Revmob::showFullscreenAd()
{
    revmob::helper::callStaticVoidMethod("showFullscreenAd");
}

void Revmob::openAdLink()
{
    revmob::helper::callStaticVoidMethod("openAdLink");
}

void Revmob::showBanner()
{
    revmob::helper::callStaticVoidMethod("showBanner");
}

void Revmob::showPopupAd()
{
    revmob::helper::callStaticVoidMethod("showPopupAd");
}

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_CONFIG_ADS_PROVIDER_REVMOB_ENABLED */