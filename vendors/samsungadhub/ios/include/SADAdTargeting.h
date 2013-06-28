//
//  SADAdTargeting.h
//  SamsungAdHub
//
//  Created by dsjang on 12. 8. 19..
//  Copyright (c) 2012년 GPMobile. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef struct SADAdTargeting {
    NSString *key;
    NSString *value;
} SADAdTargeting;

// gender
extern SADAdTargeting const male;
extern SADAdTargeting const female;

// religion
extern SADAdTargeting const buddhism;
extern SADAdTargeting const christianity;
extern SADAdTargeting const hindusm;
extern SADAdTargeting const islam;
extern SADAdTargeting const judaism;
extern SADAdTargeting const unaffiliated;
extern SADAdTargeting const others;

//  maritalstatus
extern SADAdTargeting const single;
extern SADAdTargeting const married;