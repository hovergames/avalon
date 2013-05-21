//
//  AdHubInterstitialDelegate.h
//  Samsung AdHub
//
//  Created by dsjang on 12. 8. 29..
//  Copyright (c) 2012년 dsjang. All rights reserved.
//

#import <Foundation/Foundation.h>

@class AdHubInterstitial, AdHubRequestError;
@protocol AdHubInterstitialDelegate <NSObject>
@optional

/**
@brief interstitial 광고가 화면에서 제거된 경우 호출됨
@date 2012/12/10
*/
- (void)interstitialAdDidUnload:(AdHubInterstitial *)interstitialAd;


/**
@brief interstitial 광고에서 에러가 발생한 경우 호출됨
@date 2012/12/10
*/
- (void)interstitialAd:(AdHubInterstitial *)interstitialAd didFailWithError:(AdHubRequestError *)error;

/**
@brief interstitial 광고가 로드되기 직전에 호출됨
@date 2012/12/10
*/
- (void)interstitialAdWillLoad:(AdHubInterstitial *)interstitialAd;

/**
@brief interstitial 광고가 로드 끝난 후 호출됨
@date 2012/12/10
*/
- (void)interstitialAdDidLoad:(AdHubInterstitial *)interstitialAd;


@end
