//
//  TapForTapInterstitial.h
//  TapForTapAds
//
//  Created by Sami Samhuri on 12-09-05.
//  Copyright (c) 2012 Tap for Tap. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol TapForTapInterstitialDelegate <NSObject>
@optional
- (void) tapForTapInterstitialDidReceiveAd;
- (void) tapForTapInterstitialDidShow;
- (void) tapForTapInterstitialWasDismissed;
- (void) tapForTapInterstitialFailedToDownload: (NSString *) reason;
@end

////

@interface TapForTapInterstitial : NSObject

+ (void) prepare;
+ (void) prepareWithDelegate: (id<TapForTapInterstitialDelegate>) delegate;
+ (BOOL) isReady;
+ (void) setDelegate: (id<TapForTapInterstitialDelegate>) delegate;
+ (id<TapForTapInterstitialDelegate>) delegate;
+ (void) showWithRootViewController: (UIViewController *)rootViewController;
+ (void) closeWithAnimation:(BOOL)animated;
+ (void) close;

- (BOOL) isReady;
- (void) showWithRootViewController: (UIViewController *)rootViewController;

@end
