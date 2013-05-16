#include <avalon/payment/Backend.h>

#include <avalon/payment/Manager.h>
#include <avalon/payment/ManagerDelegate.h>
#include <avalon/payment/Product.h>
#include <jni.h>
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <boost/assert.hpp>

namespace avalon {
namespace payment {

const char* const CLASS_NAME = "com/avalon/payment/Backend";
static avalon::payment::Manager* globalManager = NULL;

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
 * Java -->> C++
 */ 

extern "C" {

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_delegateOnServiceStarted(JNIEnv* env, jclass clazz)
{
    BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

    if (globalManager->delegate) {
        globalManager->delegate->onServiceStarted(globalManager);
    }
}

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_delegateOnPurchaseSucceed(JNIEnv* env, jclass clazz, jstring jProductId)
{
    BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

    if (globalManager->delegate) {
        std::string productId = cocos2d::JniHelper::jstring2string(jProductId);
        auto product = globalManager->getProduct(productId.c_str());
        if (product) {
            globalManager->delegate->onPurchaseSucceed(globalManager, product);
        }
    }
}

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_delegateOnPurchaseFail(JNIEnv* env, jclass clazz)
{
    BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

    if (globalManager->delegate) {
        globalManager->delegate->onPurchaseFail(globalManager);
    }
}

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_delegateOnTransactionStart(JNIEnv* env, jclass clazz)
{
    BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

    if (globalManager->delegate) {
        globalManager->delegate->onTransactionStart(globalManager);
    }
}

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_delegateOnTransactionEnd(JNIEnv* env, jclass clazz)
{
    BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

    if (globalManager->delegate) {
        globalManager->delegate->onTransactionEnd(globalManager);
    }
}

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_onInitialized(JNIEnv* env, jclass clazz)
{
    BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

    for (auto row : globalManager->getProducts()) {
        callStaticVoidMethodWithString("addItemDataRequest", row.second->getProductId().c_str());
    }
    callStaticVoidMethod("startItemDataRequest");
}

JNIEXPORT void JNICALL Java_com_avalon_payment_Backend_onItemData(JNIEnv* env, jclass clazz, jstring jProductId, jstring jName, jstring jDesc, jstring jPriceStr, jfloat jprice)
{
    BOOST_ASSERT_MSG(globalManager, "globalManager should be already set");

    std::string productId = cocos2d::JniHelper::jstring2string(jProductId);
    auto product = globalManager->getProduct(productId.c_str());

    if (!product) {
        return;
    }

    std::string localizedName = cocos2d::JniHelper::jstring2string(jName);
    std::string localizedDescription = cocos2d::JniHelper::jstring2string(jDesc);
    std::string localizedPrice = cocos2d::JniHelper::jstring2string(jPriceStr);

    product->localizedName = localizedName;
    product->localizedDescription = localizedDescription;
    product->localizedPrice = localizedPrice;
    product->price = (float)jprice;
}


} // extern "C"

/**
 * Public API
 */

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
    callStaticVoidMethodWithString("purchase", product->getProductId().c_str());
}

void Backend::restorePurchases() const
{
}

} // namespace payment
} // namespace avalon