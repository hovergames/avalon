//
//  AdHubConnectionManagerDelegate.h
//  SamsungAdHub
//
//  Created by dsjang on 12. 8. 19..
//  Copyright (c) 2012년 GPMobile. All rights reserved.
//

#import <Foundation/Foundation.h>

@class AdHubRequestError;
@protocol AdHubConnectionManagerDelegate <NSObject>

- (void)connectionSuccessFinished:(NSDictionary *)result;
- (void)connectionFailedFinished:(AdHubRequestError *)error;

@end
