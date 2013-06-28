//
//  SADBannerView.h
//  SamsungAdHub
//
//  Created by dsjang on 12. 8. 13..
//  Copyright (c) 2012년 GPMobile. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <avalon/platform/ios/ads/provider/samsungadhub/AdHubAdSize.h>


@protocol SADBannerViewDelegate;
@class AdHubAdORMMAView;

@interface SADBannerView : UIView {
    NSString *adUrl;
    NSTimer *refreshTimer;
    
    BOOL isRefreshing;

}

@property (nonatomic, retain) NSString *inventoryID;
@property (nonatomic, assign) AdHubAdSize adSize;
@property (nonatomic, assign) NSObject<SADBannerViewDelegate> *delegate;

//@property (nonatomic, assign) int age;
@property (nonatomic, assign) BOOL isRefreshing;

@property (nonatomic, retain) NSArray *interests;

@property (nonatomic, retain) UIViewController *rootViewController;

- (id)initWithAdSize:(AdHubAdSize)adSize origin:(CGPoint)origin;

- (void)startAd;
- (void)startAd:(NSString *)url;

//- (AdRequestParam)param;
- (AdHubAdORMMAView *)adORMMAView;

- (void)ormarViewVideoType:(int)vt;
- (void)configureTapGestureRecognizer;
- (void)deRegisterObserver;
- (void)releaseGestureRecognizer;
- (void)tapAdHubBannerView:(id)sender;
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context;
//- (void)sendSadVideoImpressionUrl;
-(void) killCarouselTimer ;



@end
