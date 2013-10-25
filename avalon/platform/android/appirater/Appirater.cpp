#include <avalon/Appirater.h>

#include <avalon/utils/platform.h>
#include <jni.h>
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"

namespace avalon {

/**
* C++ -->> Java
*/

namespace appirater {
namespace helper {

const char* const CLASS_NAME = "com/sbstrm/appirater/Appirater";

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

void callStaticVoidMethodWithBool(const char* name, const bool flag)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "(Z)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, flag);
        t.env->DeleteLocalRef(t.classID);
    }
}

void callStaticVoidMethodWithInt(const char* name, const int value)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, value);
        t.env->DeleteLocalRef(t.classID);
    }
}

} // namespace helper
} // namespace appirater

Appirater* Appirater::getInstance()
{
    static Appirater* instance = new Appirater();
    return instance;
}

void Appirater::setInitialDaysUntilPrompt(const int days)
{
    appirater::helper::callStaticVoidMethodWithInt("setInitialDaysUntilPrompt", days);
}

void Appirater::setReminderDaysUntilPrompt(const int days)
{
    appirater::helper::callStaticVoidMethodWithInt("setReminderDaysUntilPrompt", days);
}

void Appirater::setSignificantEventsUntilPrompt(const int events)
{
    appirater::helper::callStaticVoidMethodWithInt("setSignificantEventsUntilPrompt", events);
}

void Appirater::setDebug(const bool flag)
{
    appirater::helper::callStaticVoidMethodWithBool("setDebug", flag);
}

void Appirater::init()
{
#if AVALON_PLATFORM_IS_ANDROID_AMAZON
    appirater::helper::callStaticVoidMethodWithString("setMarketUrl", "http://www.amazon.com/gp/mas/dl/android?p=%s");
#else
    appirater::helper::callStaticVoidMethodWithString("setMarketUrl", "market://details?id=%s");
#endif
    appirater::helper::callStaticVoidMethod("appLaunched");
}

void Appirater::userDidSignificantEvent()
{
    appirater::helper::callStaticVoidMethod("userDidSignificantEvent");
}

void Appirater::showIfNeeded()
{
    appirater::helper::callStaticVoidMethod("showIfNeeded");
}

} // namespace avalon
