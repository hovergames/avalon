//
//  AdHubConnectionSupporter.h
//  SADAdHub
//
//  Created by dsjang on 12. 9. 7..
//  Copyright (c) 2012년 GPMobile. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <avalon/platform/ios/ads/provider/samsungadhub/AdHubAdSize.h>
#import <avalon/platform/ios/ads/provider/samsungadhub/AdHubConnectionManager.h>
#import <avalon/platform/ios/ads/provider/samsungadhub/AdHubConnectionManagerDelegate.h>

typedef enum {
    kClickToWeb = 1,
    kClickToCall,
    kClickToSamsungapps,
    kClickToMap,        //(landingpage)
    kClickToMicrosite,
    kClickToWebMap,
    kClickToMarket,     //(android)
    kClickToVideo,
    kClickToAppStore,
    kNone = 99
} AdActionType;

typedef enum {
    kAdText = 1,
    kAdImage,
    kAdVideo,
    kAdFlash,
    kAdStreaming,
    kAdExtended_Text,
    kAdCarousel,
    kAdExpandable,
    kAdInterstitial,
    kAdTV_PIA,
    kAdTV_In_app_Banner,
    kAdTV_In_app_Pre_roll,
    kAdTV_In_app_Launch_Roll,
    kAdOverlay_Video_Ad_TV_In_App,
    kAdTV_3D_Video_Ad_Banner,
    kAdPre_roll_Video_Ad,
    kAdApp_Launch_Roll_Ad,
    kAdVideo_Overlay_Banner_Ad,
    kAdVideo_Overlay_Exp_Ad
} AdType;


typedef struct SADAdReqeustParam {
    NSString *inventoryID;
    AdHubAdSize adSize;
    int age;
//    SADAdTargeting gender;
//    SADAdTargeting religion;
//    SADAdTargeting maritalStatus;
    NSArray *interests;
} AdRequestParam;



typedef struct {
    NSString *script;
    NSString *html;
} AdHtmlDocument;

@interface AdHubConnectionSupporter : NSObject {
    
    NSDictionary        *_adResult;
    AdHubConnectionManager   *_connectionManager;
}

@property (nonatomic, assign) NSObject<AdHubConnectionManagerDelegate> *connectionManagerDelegate;

- (void)sendRequest:(NSString *)url;
//- (void)sendRequest:(NSString *)url dummyParam:(AdRequestParam)param;
- (void)sendRequestWithURL:(NSString *)url;
//- (void)sendRequestAdRequest:(AdRequestParam)param;
- (void)sendRequestWithURL:(NSString *)url adRequestType:(AdRequestType)adRequestType;
- (void)sendRequestImpImpUrlGW;
- (void)sendRequestImpUrl;
- (void)sendRequestCreativeUrl;
- (void)sendRequestClickUrl;
- (void)sendRequestRdClickUrl;

- (id)adResultValueForKey:(NSString *)key;
- (id)adResultObjectValueForKey:(NSString *)key;
- (int)adResultIntValueForKey:(NSString *)key;
- (void)reconfigureAdResult:(NSDictionary *)adResult;
- (NSString *)rdUrlAppendClickUrl;
- (BOOL)hasImpUrlValue;

- (NSDictionary *)adResult;

@end
