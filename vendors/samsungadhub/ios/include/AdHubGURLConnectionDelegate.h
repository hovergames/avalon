//
//  AdHubGURLConnectionDelegate.h
//  SADAdHub
//
//  Created by dsjang on 12. 8. 22..
//  Copyright (c) 2012년 GPMobile. All rights reserved.
//

#import <Foundation/Foundation.h>

@class AdHubGURLConnection;
@protocol AdHubGURLConnectionDelegate <NSObject>

@optional

- (void)connection:(AdHubGURLConnection *)connection didFailWithError:(NSError *)error;
- (void)connectionDidFinishLoading:(AdHubGURLConnection *)connection;

@end
