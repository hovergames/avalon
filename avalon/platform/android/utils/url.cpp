#include <avalon/utils/url.h>

#include <jni.h>
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"

namespace avalon {
namespace utils {
namespace url {

void open(const char* url)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/avalon/utils/Url", "open", "(Ljava/lang/String;)V")) {
        jstring jUrl = t.env->NewStringUTF(url);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jUrl);
    }
}

} // namespace url
} // namespace utils
} // namespace avalon
