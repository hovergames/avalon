#ifdef AVALON_CONFIG_ADS_PROVIDER_CHARTBOOST_ENABLED

#include <avalon/ads/provider/Chartboost.h>

#include <jni.h>
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <boost/assert.hpp>

using namespace cocos2d;

namespace avalon {
namespace ads {
namespace provider {

/**
* C++ -->> Java
*/

namespace chartboost {
namespace helper {

const char* const CLASS_NAME = "com/avalon/ads/ChartboostBridge";

void callStaticVoidMethod(const char* name)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void callStaticVoidMethodWithString(const char* name, const char* arg1, const char* arg2)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "(Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring jArg1 = t.env->NewStringUTF(arg1);
        jstring jArg2 = t.env->NewStringUTF(arg2);

        t.env->CallStaticVoidMethod(t.classID, t.methodID, jArg1, jArg2);
        t.env->DeleteLocalRef(t.classID);

        t.env->DeleteLocalRef(jArg1);
        t.env->DeleteLocalRef(jArg2);
    }
}

} // namespace helper
} // namespace chartboost

Chartboost::Chartboost()
: appId()
, appSignature()
{
}

void Chartboost::init()
{
    BOOST_ASSERT_MSG(!appId.empty(), "appId is empty!");
    BOOST_ASSERT_MSG(!appSignature.empty(), "appSignature is empty!");

    chartboost::helper::callStaticVoidMethodWithString("init", appId.c_str(), appSignature.c_str());
}

void Chartboost::hideAds()
{
    chartboost::helper::callStaticVoidMethod("hideAds");
}

void Chartboost::showFullscreenAd()
{
    chartboost::helper::callStaticVoidMethod("showFullscreenAd");
}

void Chartboost::openAdLink()
{
    chartboost::helper::callStaticVoidMethod("openAdLink");
}

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_CONFIG_ADS_PROVIDER_CHARTBOOST_ENABLED */