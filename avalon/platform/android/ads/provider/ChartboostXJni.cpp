/****************************************************************************
 Copyright (c) 2012      Wenbin Wang
 
 http://geeksavetheworld.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include <avalon/platform/android/ads/provider/ChartboostXJni.h>
#include <avalon/ads/provider/ChartboostX.h>

#define  CLASS_NAME "com/wenbin/ChartboostX/ChartboostXBridge"

using namespace cocos2d;

extern "C"
{
    
    void setAppIdJNI(const char * appId)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "setAppId", "(Ljava/lang/String;)V"))
        {
            return;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(appId);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
        
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    
    void setAppSignatureJNI(const char * appSignature)
    {
        JniMethodInfo methodInfo;
        
        if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "setAppSignature", "(Ljava/lang/String;)V"))
        {
            return;
        }
        jstring stringArg = methodInfo.env->NewStringUTF(appSignature);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
        
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(stringArg);
    }
    
    void installJNI()
    {
        JniMethodInfo methodInfo;
        
        if (!  JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "install", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void cacheInterstitialJNI(const char* location)
    {
        JniMethodInfo methodInfo;
        
        if (location) {
            if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "cacheInterstitial", "(Ljava/lang/String;)V"))
            {
                return;
            }
            jstring stringArg = methodInfo.env->NewStringUTF(location);
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
            
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
            methodInfo.env->DeleteLocalRef(stringArg);
        } else {
            if (!  JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "cacheInterstitial", "()V"))
            {
                return;
            }
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
    }
    
    void showInterstitialJNI(const char* location)
    {
        JniMethodInfo methodInfo;

        if (location) {
            if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "showInterstitial", "(Ljava/lang/String;)V"))
            {
                return;
            }
            jstring stringArg = methodInfo.env->NewStringUTF(location);
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
            
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
            methodInfo.env->DeleteLocalRef(stringArg);
        } else {
            if (!  JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "showInterstitial", "()V"))
            {
                return;
            }
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
    }
    
    void hasCachedInterstitialJNI(const char* location)
    {
        JniMethodInfo methodInfo;
        
        if (location) {
            if (! JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "hasCachedInterstitial", "(Ljava/lang/String;)V"))
            {
                return;
            }
            jstring stringArg = methodInfo.env->NewStringUTF(location);
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
            
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
            methodInfo.env->DeleteLocalRef(stringArg);
        } else {
            if (!  JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "hasCachedInterstitial", "()V"))
            {
                return;
            }
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
    }
    
    void cacheMoreAppsJNI()
    {
        JniMethodInfo methodInfo;
        
        if (!  JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "cacheMoreApps", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    void showMoreAppsJNI()
    {
        JniMethodInfo methodInfo;
        
        if (!  JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "showMoreApps", "()V"))
        {
            return;
        }
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    jboolean Java_com_wenbin_ChartboostX_ChartboostXBridge_shouldRequestInterstitial(JNIEnv*  env, jobject thiz, jstring location)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            return delegate->shouldRequestInterstitial(JniHelper::jstring2string(location).c_str());
        }
        
        return true;
    }
    
    jboolean Java_com_wenbin_ChartboostX_ChartboostXBridge_shouldDisplayInterstitial(JNIEnv*  env, jobject thiz, jstring location)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            return delegate->shouldDisplayInterstitial(JniHelper::jstring2string(location).c_str());
        }
        
        return true;
    }

    jboolean Java_com_wenbin_ChartboostX_ChartboostXBridge_shouldRequestInterstitialsInFirstSession(JNIEnv*  env, jobject thiz)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            return delegate->shouldRequestInterstitialsInFirstSession();
        }

        return true;
    }

    void Java_com_wenbin_ChartboostX_ChartboostXBridge_didCacheInterstitial(JNIEnv*  env, jobject thiz, jstring location)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            delegate->didCacheInterstitial(JniHelper::jstring2string(location).c_str());
        }
    }
    
    void Java_com_wenbin_ChartboostX_ChartboostXBridge_didFailToLoadInterstitial(JNIEnv*  env, jobject thiz, jstring location)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            delegate->didFailToLoadInterstitial(JniHelper::jstring2string(location).c_str());
        }
    }
    
    void Java_com_wenbin_ChartboostX_ChartboostXBridge_didDismissInterstitial(JNIEnv*  env, jobject thiz, jstring location)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            delegate->didDismissInterstitial(JniHelper::jstring2string(location).c_str());
        }
    }
    
    void Java_com_wenbin_ChartboostX_ChartboostXBridge_didCloseInterstitial(JNIEnv*  env, jobject thiz, jstring location)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            delegate->didCloseInterstitial(JniHelper::jstring2string(location).c_str());
        }
    }
    
    void Java_com_wenbin_ChartboostX_ChartboostXBridge_didClickInterstitial(JNIEnv*  env, jobject thiz, jstring location)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            delegate->didClickInterstitial(JniHelper::jstring2string(location).c_str());
        }
    }
    
    jboolean Java_com_wenbin_ChartboostX_ChartboostXBridge_shouldDisplayLoadingViewForMoreApps(JNIEnv*  env, jobject thiz)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            return delegate->shouldDisplayLoadingViewForMoreApps();
        }
        
        return true;
    }
    
    jboolean Java_com_wenbin_ChartboostX_ChartboostXBridge_shouldRequestMoreApps(JNIEnv*  env, jobject thiz)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            return delegate->shouldRequestMoreApps();
        }
        
        return true;
    }
    
    jboolean Java_com_wenbin_ChartboostX_ChartboostXBridge_shouldDisplayMoreApps(JNIEnv*  env, jobject thiz)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            return delegate->shouldDisplayMoreApps();
        }
        
        return true;
    }
    
    void Java_com_wenbin_ChartboostX_ChartboostXBridge_didFailToLoadMoreApps(JNIEnv*  env, jobject thiz)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            delegate->didFailToLoadMoreApps();
        }
    }
    
    void Java_com_wenbin_ChartboostX_ChartboostXBridge_didDismissMoreApps(JNIEnv*  env, jobject thiz)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            delegate->didDismissMoreApps();
        }
    }
    
    void Java_com_wenbin_ChartboostX_ChartboostXBridge_didCloseMoreApps(JNIEnv*  env, jobject thiz)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            delegate->didCloseMoreApps();
        }
    }
    
    void Java_com_wenbin_ChartboostX_ChartboostXBridge_didClickMoreApps(JNIEnv*  env, jobject thiz)
    {
        if (ChartboostXDelegate* delegate = ChartboostX::sharedChartboostX()->getDelegate()) {
            delegate->didClickMoreApps();
        }
    }
}
