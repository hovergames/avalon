//
//  TapForTapAdMobBanner.h
//  TapForTap
//
//  Created by Sami Samhuri on 12-06-25.
//  Copyright (c) 2012 Tap for Tap. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GADCustomEventBanner.h"
#import "TapForTap.h"

@interface TapForTapAdMobBanner : NSObject <GADCustomEventBanner, TapForTapAdViewDelegate>

@property (nonatomic, assign) id<GADCustomEventBannerDelegate> delegate;
@property (nonatomic, retain) TapForTapAdView *adView;

@end
