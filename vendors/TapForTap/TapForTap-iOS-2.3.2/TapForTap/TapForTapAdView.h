//
//  TapForTapAdView.h
//  TapForTapAds
//
//  Created by Sami Samhuri on 12-02-06.
//  Copyright (c) 2012 Tap for Tap. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class TapForTapAdView;

@protocol TapForTapAdViewDelegate <NSObject>

@optional
- (UIViewController *) rootViewController;
- (void) tapForTapAdViewDidReceiveAd: (TapForTapAdView *)adView;
- (void) tapForTapAdView: (TapForTapAdView *)adView didFailToReceiveAd: (NSString *)reason;
- (void) tapForTapAdViewWasTapped: (TapForTapAdView *)adView;

@end

////

@interface TapForTapAdView : UIView

@property (nonatomic, assign) id<TapForTapAdViewDelegate> delegate;
@property (nonatomic, assign) BOOL autoRollover;

- (id) initWithFrame: (CGRect)frame delegate: (id<TapForTapAdViewDelegate>)delegate;
- (void) loadAds;
- (void) stopLoadingAds;

@end
