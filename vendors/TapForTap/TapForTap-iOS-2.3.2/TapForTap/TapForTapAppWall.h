//
//  TapForTapAppWall.h
//  TapForTapAds
//
//  Created by Sami Samhuri on 12-09-05.
//  Copyright (c) 2012 Tap for Tap. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol TapForTapAppWallDelegate <NSObject>
@optional
- (void) tapForTapAppWallDidReceiveAd;
- (void) tapForTapAppWallDidShow;
- (void) tapForTapAppWallWasDismissed;
- (void) tapForTapAppWallFailedToDownload: (NSString *) reason;
@end

////

@interface TapForTapAppWall : NSObject

+ (void) prepare;
+ (void) prepareWithDelegate: (id<TapForTapAppWallDelegate>) delegate;
+ (BOOL) isReady;
+ (void) setDelegate: (id<TapForTapAppWallDelegate>) delegate;
+ (id<TapForTapAppWallDelegate>) delegate;
+ (void) showWithRootViewController: (UIViewController *)rootViewController;
+ (void) closeWithAnimation:(BOOL)animated;
+ (void) close;

- (BOOL) isReady;
- (void) showWithRootViewController: (UIViewController *)rootViewController;

@end
