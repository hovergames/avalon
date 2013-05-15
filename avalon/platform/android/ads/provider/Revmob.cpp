#include <avalon/ads/provider/Revmob.h>

#include <jni.h>
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <boost/assert.hpp>

const char* const CLASS_NAME = "com/avalon/ads/RevmobBridge";

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

/**
 * Public API
 */
 
namespace avalon {
namespace ads {
namespace provider {

Revmob::Revmob()
: appId()
{
}

void Revmob::init()
{
    BOOST_ASSERT_MSG(appId != "", "appId must be set first");
    callStaticVoidMethodWithString("init", appId.c_str());
}

void Revmob::hideAds()
{
    callStaticVoidMethod("hideAds");
}

void Revmob::showFullscreenAd()
{
    callStaticVoidMethod("showFullscreenAd");
}

void Revmob::openAdLink()
{
    callStaticVoidMethod("openAdLink");
}

void Revmob::showBanner()
{
    callStaticVoidMethod("showBanner");
}

void Revmob::showPopupAd()
{
    callStaticVoidMethod("showPopupAd");
}

} // namespace provider
} // namespace ads
} // namespace avalon