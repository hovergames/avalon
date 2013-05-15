#include <avalon/payment/Backend.h>

#include <avalon/payment/Manager.h>
#include <avalon/payment/ManagerDelegate.h>
#include <avalon/payment/Product.h>
#include <jni.h>
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"

const char* const CLASS_NAME = "com/avalon/payment/Backend";
static avalon::payment::Manager* globalManager = NULL;

/**
 * Java -->> C++
 */

extern "C" {

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_delegateOnServiceStarted(JNIEnv* env, jclass clazz)
{
    if (globalManager && globalManager->delegate) {
        globalManager->delegate->onServiceStarted(globalManager);
    }
}

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_delegateOnPurchaseSucceed(JNIEnv* env, jclass clazz, jstring jProductId)
{
    if (globalManager && globalManager->delegate) {
        std::string productId = cocos2d::JniHelper::jstring2string(jProductId);
        globalManager->delegate->onPurchaseSucceed(globalManager, globalManager->getProduct(productId.c_str()));
    }
}

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_delegateOnPurchaseFail(JNIEnv* env, jclass clazz)
{
    if (globalManager && globalManager->delegate) {
        globalManager->delegate->onPurchaseFail(globalManager);
    }
}

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_delegateOnTransactionStart(JNIEnv* env, jclass clazz)
{
    if (globalManager && globalManager->delegate) {
        globalManager->delegate->onTransactionStart(globalManager);
    }
}

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_delegateOnTransactionEnd(JNIEnv* env, jclass clazz)
{
    if (globalManager && globalManager->delegate) {
        globalManager->delegate->onTransactionEnd(globalManager);
    }
}

} // extern "C"

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

bool callStaticBoolMethod(const char* name)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, name, "()Z")) {
        bool result = (t.env->CallStaticBooleanMethod(t.classID, t.methodID) == JNI_TRUE);
        t.env->DeleteLocalRef(t.classID);
        return result;
    } else {
        return false;
    }
}

/**
 * Public API
 */

namespace avalon {
namespace payment {

Backend::Backend(Manager& manager)
: manager(manager)
{
    globalManager = &manager;
}

Backend::~Backend()
{
    shutdown();
    globalManager = NULL;
}

void Backend::shutdown()
{
    callStaticVoidMethod("shutdown");
}

void Backend::initialize()
{
    callStaticVoidMethod("initialize");
}

bool Backend::isInitialized() const
{
    return callStaticBoolMethod("isInitialized");
}

bool Backend::isPurchaseReady() const
{
    return callStaticBoolMethod("isPurchaseReady");
}

void Backend::purchase(Product* const product)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "purchase", "(Ljava/lang/String;)V")) {
        jstring jProductId = t.env->NewStringUTF(product->getProductId().c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jProductId);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Backend::restorePurchases() const
{
}

} // namespace payment
} // namespace avalon