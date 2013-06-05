//
//  AdHubView.h
//  SADAdHub
//
//  Created by gpmobile on 12. 11. 30..
//  Copyright (c) 2012년 dsjang. All rights reserved.
//

#import <avalon/platform/ios/ads/provider/samsungadhub/SADBannerView.h>
#import <avalon/platform/ios/ads/provider/samsungadhub/AdHubUserProfile.h>



@class AdHubParameter;

@interface AdHubView : SADBannerView {
    AdHubParameter *param;
    id delegate;
    UIViewController *rootViewController;
    
    NSTimer *retryTimer;
    NSString *profileStr;
}

/**
@brief 생성자
@date 2012/12/10
@param adSize 배너크기를 지정하는 상수
@param origin 배너의 좌측 상단 좌표
@param inventoryID 인벤토리 id
@return 배너뷰 객체
*/
- (id) initWithAdSize:(AdHubAdSize)adSize origin:(CGPoint)origin inventoryID:(NSString*)inventoryID;



/**
@brief user profile 설정
@date 2012/12/10
@param p user profile 객체
@remark 광고소비자의 나이,성별,종교,최미,관심사 설정
*/
- (void) setUserProfile:(AdHubUserProfile *)p;


/**
@brief refresh timer 갱신 주기 설정
@date 2012/12/10
@param time 시간, 단위는 ms 1/1000초
@remark 사용자는 광고 갱신 주기를 설정할수 있다, 설정하지 않는 경우 서버의 설정 값 사용, 최소값은 12초
*/
- (void) setRefreshRate:(int)time;



/**
 @brief refresh timer 재시작
 @date 2012/12/10
 */
- (void)startRefresh;

/**
 @brief refresh timer 정지
 @date 2012/12/10
 */
-(void)stopRefresh;



-(void)setMcc:(NSString *)mcc;


/**
@brief 이벤트를 처리할 delegate 객체
@date 2012/12/10
*/
@property (nonatomic, assign) id delegate;



@property (nonatomic, retain) UIViewController *rootViewController;


@end
