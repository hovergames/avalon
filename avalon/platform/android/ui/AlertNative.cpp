#include <avalon/ui/Alert.h>

#include <avalon/ui/AlertDelegate.h>
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

namespace avalon {
namespace ui {

const char* const CLASS_NAME = "com/avalon/ui/Alert";

void showAlert(const std::string& title, const std::string& message, const ButtonList& buttons, AlertDelegate* delegate)
{
    // reset any previously shared stuff
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "reset", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }

    // share buttons with the java side
    for (auto& entry : buttons) {
        if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "addButton", "(ILjava/lang/String;)V")) {
            jstring jLabel = t.env->NewStringUTF(entry.second.c_str());
            
            t.env->CallStaticVoidMethod(t.classID, t.methodID, (jint)entry.first, jLabel);
            t.env->DeleteLocalRef(t.classID);
            
            t.env->DeleteLocalRef(jLabel);
        }
    }

    // create and show the alert
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "show", "(Ljava/lang/String;Ljava/lang/String;J)V")) {
        jstring jTitle = t.env->NewStringUTF(title.c_str());
        jstring jMessage = t.env->NewStringUTF(message.c_str());
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jTitle, jMessage, (jlong)delegate);
        t.env->DeleteLocalRef(t.classID);
        
        t.env->DeleteLocalRef(jTitle);
        t.env->DeleteLocalRef(jMessage);
    }
}

extern "C" {

JNIEXPORT void JNICALL Java_com_avalon_ui_Alert_onClick(JNIEnv* env, jclass clazz, jlong delegatePtr, jint jIndex, jstring jLabel)
{
    AlertDelegate* delegate = (AlertDelegate*)delegatePtr;
    if (!delegate) {
        return;
    }

    unsigned int index = (unsigned int)jIndex;
    std::string label = cocos2d::JniHelper::jstring2string(jLabel);
    delegate->onAlertButtonClick(index, label);
}

}

} // namespace ui
} // namespace avalon