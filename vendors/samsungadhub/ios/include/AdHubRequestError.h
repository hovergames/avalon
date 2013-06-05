//
//  AdHubRequestError.h
//  SADAdHub
//
//  Created by dsjang on 12. 8. 28..
//  Copyright (c) 2012년 GPMobile. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString *const kSADErrorDomain;

typedef enum {
    kSADErrorUnknown,
    
    kSADError1000,
    kSADError1001,
    kSADError1002,
    kSADError1003,
    kSADError1004,
    
    kSADError2000,
    kSADError2001,
    kSADError2002,
    
    kSADError9001,
    kSADError9002,
    kSADError9003,
} SADErrorCode;

@interface AdHubRequestError : NSError

+ (id)errorWithDomain:(NSString *)domain SADCode:(NSString *)code userInfo:(NSDictionary *)dict;
+ (SADErrorCode)convertErrorCode:(NSString *)code ;
//+ (SADErrorCode)convertErrorCode2:(NSString *)code;

@end
