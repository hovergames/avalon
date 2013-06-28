//
//  ExampleAppDelegate.m
//  TapForTapAds
//
//  Copyright (c) 2012 Beta Street. All rights reserved.
//

#import "ExampleAppDelegate.h"
#import "TapForTap.h"

@implementation ExampleAppDelegate

- (BOOL) application: (UIApplication *)application didFinishLaunchingWithOptions: (NSDictionary *)launchOptions
{
    [TapForTap initializeWithAPIKey: @"YOUR API KEY"];

    // Set up the main window and root view controller

    return YES;
}

@end
