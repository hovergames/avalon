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

#include <avalon/ads/provider/ChartboostX.h>
#include <avalon/platform/android/ads/provider/ChartboostXJni.h>

static ChartboostX* s_pChartboostX = NULL;

ChartboostX* ChartboostX::sharedChartboostX()
{
    if (s_pChartboostX == NULL)
    {
        s_pChartboostX = new ChartboostX();
    }
    return s_pChartboostX;
}

void ChartboostX::purgeChartboostX()
{
    CC_SAFE_DELETE(s_pChartboostX);
}

void ChartboostX::setAppId(const char * appId)
{
    setAppIdJNI(appId);
}

void ChartboostX::setAppSignature(const char * appSignature)
{
    setAppSignatureJNI(appSignature);
}

void ChartboostX::startSession()
{
    installJNI();
}

void ChartboostX::cacheInterstitial(const char* location)
{
    cacheInterstitialJNI(location);
}

void ChartboostX::showInterstitial(const char* location)
{
    showInterstitialJNI(location);
}

bool ChartboostX::hasCachedInterstitial(const char* location)
{
    hasCachedInterstitialJNI(location);
    
    return false;
}

void ChartboostX::cacheMoreApps()
{
    cacheMoreAppsJNI();
}

void ChartboostX::showMoreApps()
{
    showMoreAppsJNI();
}
