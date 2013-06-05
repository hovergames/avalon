//
//  SADInterstitialAd.h
//  SADAdHub
//
//  Created by dsjang on 12. 8. 29..
//  Copyright (c) 2012년 GPMobile. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol SADInterstitialAdDelegate;
@interface SADInterstitialAd : NSObject

@property (nonatomic, assign) NSObject<SADInterstitialAdDelegate> *delegate;
@property (nonatomic, retain) NSString *inventoryID;

- (void)presentFromViewController:(UIViewController *)viewController url:(NSString *)url;
- (void)presentFromViewController:(UIViewController *)viewController;

@end
