//
//  AdHubViewDelegate.h
//  Samsung AdHub
//
//  Created by dsjang on 12. 8. 28..
//  Copyright (c) 2012년 dsjang. All rights reserved.
//

#import <Foundation/Foundation.h>

@class AdHubView, AdHubRequestError;
@protocol AdHubViewDelegate <NSObject>

@optional
//- (void)bannerViewWillLoadAd:(AdHubView *)banner;


/**
@brief 배너 광고가 로드되면 호출 됨
@date 2012/12/10
*/
- (void)bannerViewDidLoadAd:(AdHubView *)banner;

/**
@brief 배너 광고에서 에러가 발생하면 호출 됨
@date 2012/12/10
*/
- (void)bannerView:(AdHubView *)banner didFailToReceiveAdWithError:(AdHubRequestError *)error;
//- (BOOL)bannerViewActionShouldBegin:(AdHubView *)banner willLeaveApplication:(BOOL)willLeave;
//- (void)bannerViewActionDidFinish:(AdHubView *)banner;
//- (void)bannerViewActionDidClose:(AdHubView *)banner;
@end
