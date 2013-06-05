//
//  AdHubInterstitial.h
//  AdHubTest2
//
//  Created by gpmobile on 12. 11. 30..
//  Copyright (c) 2012년 gpmobile. All rights reserved.
//

#import "SADInterstitialAd.h"
#import "AdHubParameter.h"
#import "AdHubUserProfile.h"


@interface AdHubInterstitial : SADInterstitialAd {
    AdHubParameter *param;


    id delegate;
    NSString *profileStr;
}

/**
@brief 생성자
@date 2012/12/10
@param inventoryID 인벤토리 id
@return interstitial 객체
*/
-(id) initWithInventoryID:(NSString *)inventoryID;

/**
@brief interstitial 광고를 표시
@date 2012/12/10
@param viewController interstitial광고를 불러올 view controller 객체
*/
-(void) presentFromViewController:(UIViewController*) viewController;


/**
@brief user profile 설정
@date 2012/12/10
@param p user profile 객체
@remark 광고소비자의 나이,성별,종교,최미,관심사 설정
*/
-(void) setUserProfile:(AdHubUserProfile *)p;

/**
@brief 이벤트를 처리할 delegate 객체
@date 2012/12/10
*/
@property (nonatomic, assign) id delegate;

@end
