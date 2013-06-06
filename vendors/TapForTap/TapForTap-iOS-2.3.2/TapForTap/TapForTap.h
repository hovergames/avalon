//
//  TapForTap.h
//  TapForTapAds
//
//  Created by Sami Samhuri on 12-03-12.
//  Copyright (c) 2012 Tap for Tap. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "TapForTapAdView.h"
#import "TapForTapAppWall.h"
#import "TapForTapInterstitial.h"

#define TFT_SDK_VERSION @"2.3.2"

typedef enum {
    NONE = -1,
    MALE = 0,
    FEMALE = 1
} TapForTapGender;


@interface TapForTap : NSObject

+ (void) initializeWithAPIKey: (NSString *)apiKey;
+ (BOOL) testMode;
+ (void) setTestMode: (BOOL)testMode;
+ (void) setGender: (TapForTapGender)gender;
+ (void) setAge: (NSUInteger)age;
+ (void) setYearOfBirth: (NSUInteger)year;
+ (void) setLocation: (CLLocation *)location;
+ (void) setUserAccountID: (NSString *)accountId;

@end
