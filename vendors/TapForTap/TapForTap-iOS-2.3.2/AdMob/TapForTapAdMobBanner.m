//
//  TapForTapAdMobBanner.m
//  TapForTap
//
//  Created by Sami Samhuri on 12-06-25.
//  Copyright (c) 2012 Tap for Tap. All rights reserved.
//

#import "TapForTapAdMobBanner.h"

@implementation TapForTapAdMobBanner

@synthesize delegate = _delegate;
@synthesize adView = _adView;

#pragma mark -
#pragma mark GADCustomEventBanner

- (void)requestBannerAd:(GADAdSize)adSize
              parameter:(NSString *)serverParameter
                  label:(NSString *)serverLabel
                request:(GADCustomEventRequest *)request  {

    if (!self.adView) {
        if (serverParameter) {
            [TapForTap initializeWithAPIKey: serverParameter];
        }
        self.adView = [[TapForTapAdView alloc] initWithFrame: CGRectMake(0, 0, 320, 50) delegate: self];
        self.adView.autoRollover = NO;
    }
}

#pragma mark -
#pragma mark TapForTapAdView Callbacks

- (void) tapForTapAdViewDidReceiveAd: (TapForTapAdView *)adView
{
    [self.adView performSelector: @selector(setDelegate:) withObject: nil afterDelay: 30.0];
    [self.delegate customEventBanner: self didReceiveAd: adView];
}

- (void) tapForTapAdViewWasTapped: (TapForTapAdView *)adView
{
    [self.delegate customEventBanner: self clickDidOccurInAd: self.adView];
}

- (void) tapForTapAdView: (TapForTapAdView *)adView didFailToReceiveAd: (NSString *)reason
{
    self.adView.delegate = nil;
    [self.delegate customEventBanner: self didFailAd: nil];
}

- (UIViewController *) rootViewController
{
    return [self.delegate viewControllerForPresentingModalView];
}

- (void) dealloc
{
    _adView.delegate = nil;
    [_adView release];
    _adView = nil;
    _delegate = nil;
    [super dealloc];
}

@end
