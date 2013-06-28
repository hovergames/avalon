//
//  AdHubConnectionManager.h
//  SamsungAdHub
//
//  Created by dsjang on 12. 8. 17..
//  Copyright (c) 2012년 GPMobile. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <avalon/platform/ios/ads/provider/samsungadhub/AdHubConnectionManagerDelegate.h>
#import <avalon/platform/ios/ads/provider/samsungadhub/AdHubGURLConnectionDelegate.h>

typedef enum {
    kAdRequestTypeNone,
    kAdRequest,
    kAdImpUrlGW,
    kAdImpUrl,
    kAdClick,
    kAdRdURL,
    kAdRdClick,
    kAdCreativeUrl
} AdRequestType;

@class AdHubGURLConnection;
@interface AdHubConnectionManager : NSObject<AdHubGURLConnectionDelegate> {
    
    AdHubGURLConnection     *_connection;
}

@property (nonatomic, assign) NSObject<AdHubConnectionManagerDelegate> *delegate;

- (void)sendRequest:(NSString *)url adRequestType:(AdRequestType)adRequestType;
- (void)connectionDidFinishLoading:(AdHubGURLConnection *)adConnection;
- (void)connection:(AdHubGURLConnection *)adConnection didFailWithError:(NSError *)error;
- (NSDictionary *)parsingSuccessDataFromAdConnection:(AdHubGURLConnection *)adConnection;
- (NSDictionary *)parsingFailedDataFromAdConnection:(AdHubGURLConnection *)adConnection;
- (void)sendSuccessMessageToDelegateWithAdResponse:(NSDictionary *)result;
- (void)sendFailedMessageToDelegateWithAdRequestError:(AdHubRequestError *)error;
- (NSDictionary *)errorUserInfoWithdescription:(NSObject *)description failureReasonError:(NSObject *)failureReasonError;
- (void)sendFailedMessageToDelegateWithNSError:(NSError *)error;
- (AdHubRequestError *)SADReqeustErrorFromNSError:(NSError *)error;
- (void)sendFailedMessageToDelegateWithAdError:(NSDictionary *)error;
- (AdHubRequestError *)SADReqeustErrorFromNSDictionary:(NSDictionary *)error;



extern NSString *const kLocalStringKey;
extern NSString *const kAdRequestTypeKey;
extern NSString *const kStatusCodeKey;
extern NSString *const kReceivedDataKey;
extern NSString *const kAllHeaderFieldsKey;

@end
