//
//  TapForTapAdMobInterstitial.m
//  TapForTapAds
//
//  Created by Phil Laird on 2013-04-02.
//  Copyright (c) 2013 Tap for Tap. All rights reserved.
//

#import "TapForTapAdMobInterstitial.h"


@implementation TapForTapAdMobInterstitial

@synthesize delegate = _delegate;

- (void)requestInterstitialAdWithParameter:(NSString *)serverParameter
                                     label:(NSString *)serverLabel
                                   request:(GADCustomEventRequest *)request
{
    if([TapForTapInterstitial isReady]) {
        [self performSelector:@selector(tapForTapInterstitialDidReceiveAd) withObject:nil afterDelay:0.5];
    } else {
        [TapForTapInterstitial prepareWithDelegate:self];
    }
}

- (void)presentFromRootViewController:(UIViewController *)rootViewController
{
    [TapForTapInterstitial showWithRootViewController:rootViewController];
}

- (void) tapForTapInterstitialDidReceiveAd
{
    [self.delegate customEventInterstitial:self didReceiveAd:nil];
}

- (void) tapForTapInterstitialDidShow
{
    [self.delegate customEventInterstitialWillPresent:self];
}

- (void) tapForTapInterstitialWasDismissed
{
    [self.delegate customEventInterstitialDidDismiss:self];
}

- (void) tapForTapInterstitialFailedToDownload:(NSString *) reason
{
    [self.delegate customEventInterstitial:self didFailAd:nil];
}

@end
