//
//  Parameter.h
//  SADAdHub
//
//  Created by gpmobile on 12. 11. 13..
//  Copyright (c) 2012년 GPMobile. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CoreTelephony/CTCarrier.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>



@interface AdHubParameter : NSObject {
    NSString *paramInventoryID;
    NSString *paramQuery;
    NSString *paramInventorySize;
    NSString *paramDt;
    NSString *paramPt;
    NSString *paramMcc;
    NSString *paramCoc;
    NSString *paramPv;
    NSString *paramI;
    NSString *paramScr;
    NSString *paramDid;
    NSString *paramNt;
    NSString *paramCel;
    NSString *paramLac;
    NSString *paramMnc;
    NSString *paramPoc;
    NSString *paramLnc;
    NSString *paramTs;
    NSString *paramTz;
    NSString *paramMf;
    NSString *paramSdk;
//    NSString *paramLocation;
    
}
-(void) setParam ;
-(NSString*) getQuery ;

@property (nonatomic, assign) NSString *paramInventoryID;
@property (nonatomic, assign) NSString *paramInventorySize;
@property (nonatomic, assign) NSString *paramMcc;

@end
