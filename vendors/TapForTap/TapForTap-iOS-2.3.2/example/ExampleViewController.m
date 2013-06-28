//
//  ExampleViewController.m
//  TapForTapAds
//
//  Copyright (c) 2012 Tap for Tap. All rights reserved.
//

#import "ExampleViewController.h"
#import "TapForTap.h"

@implementation ExampleViewController

@synthesize adView = _adView;

- (void) viewDidLoad
{
    [super viewDidLoad];

    // Show a banner at the bottom of this view, 320x50 points
    CGFloat y = self.view.frame.size.height - 50.0;
    self.adView = [[TapForTapAdView alloc] initWithFrame: CGRectMake(0, y, 320.0, 50.0) delegate: self];
    [self.view addSubview: self.adView];

    // Prepare interstitial and app wall, if desired
    [TapForTapInterstitial prepare];
    [TapForTapAppWall prepare];
}

// Hook up a button to this method to show an interstitial, or call it before pushing a view controller, etc.
- (IBAction) showInterstitial: (id)sender
{
    // Show an Interstitial
    [TapForTapInterstitial showWithRootViewController: self];
}

// Hook up a button to this method to show an app wall
- (IBAction) showMoreApps: (id)sender
{
    // Show an App Wall
    [TapForTapAppWall showWithRootViewController: self];
}

#pragma mark - TapForTapAdViewDelegate methods

- (UIViewController *) rootViewController
{
    return self; // or possibly self.navigationController
}

- (void) tapForTapAdViewDidReceiveAd: (TapForTapAdView *)adView
{
    NSLog(@"ad view did receive ad");
}

- (void) tapForTapAdView: (TapForTapAdView *)adView didFailToReceiveAd: (NSString *)reason
{
    NSLog(@"ad view failed to load ads: %@", reason);
}

- (void) tapForTapAdViewWasTapped: (TapForTapAdView *)adView
{
    NSLog(@"ad view tapped");
}

@end
