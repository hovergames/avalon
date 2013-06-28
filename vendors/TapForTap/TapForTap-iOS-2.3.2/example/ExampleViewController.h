//
//  ExampleViewController.h
//  TapForTapAds
//
//  Copyright (c) 2012 Tap for Tap. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "TapForTap.h"

@interface ExampleViewController : UIViewController <TapForTapAdViewDelegate>

@property (nonatomic, retain) TapForTapAdView *adView;

@end
