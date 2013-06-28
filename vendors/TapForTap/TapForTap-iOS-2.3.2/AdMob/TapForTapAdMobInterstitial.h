//
//  TapForTapAdMobInterstitial.h
//  TapForTapAds
//
//  Created by Phil Laird on 2013-04-02.
//  Copyright (c) 2013 Tap for Tap. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GADCustomEventInterstitial.h"
#import "GADCustomEventInterstitialDelegate.h"
#import "TapForTap.h"

@interface TapForTapAdMobInterstitial : NSObject <GADCustomEventInterstitial, TapForTapInterstitialDelegate>

@property (nonatomic, assign) id<GADCustomEventInterstitialDelegate> delegate;

@end
