//
//  AdHubAdSize.h
//  SamsungAdHub
//
//  Created by dsjang on 12. 8. 19..
//  Copyright (c) 2012년 GPMobile. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef struct AdHubAdSize {
    CGSize  size;
    NSUInteger adType;
} AdHubAdSize;

extern AdHubAdSize const kAdHubAdSize_B_300x250;
extern AdHubAdSize const kAdHubAdSize_B_320x48;
extern AdHubAdSize const kAdHubAdSize_B_728x90;
extern AdHubAdSize const kAdHubAdSize_I_320x480;
extern AdHubAdSize const kAdHubAdSize_I_768x1024;
extern AdHubAdSize const kAdHubAdSize_I_1024x768;
