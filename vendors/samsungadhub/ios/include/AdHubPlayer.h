//
//  adHubPlayer.h
//  SADAdHub
//
//  Created by gpmobile on 12. 11. 30..
//  Copyright (c) 2012년 dsjang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AdHubUserProfile.h"
#import "AdHubAdSize.h"

@class AdHubParameter;
@class SADPlayerConnection;
@interface AdHubPlayer : NSObject
{
    NSString * strInventoryID;
    AdHubParameter *param;
    SADPlayerConnection *movieConnecter;
    id delegate;
    NSString *profileStr;
}
@property (nonatomic, assign)id delegate;


/**
@brief 생성자
@date 2012/12/10
@param strId 인벤토리 id
@return VAST player 객체
*/
- (id)initWithInventoryID:(NSString *)strId;


/**
@brief App launch roll 광고 시작
@date 2012/12/10
*/
- (void)startAdAppLaunchRoll;

/**
@brief pre roll 광고 시작
@date 2012/12/10
@param videoUrl 동영상 주소
*/
- (void)startAdPreRollWithContentURL:(NSString *)videoUrl;

/**
@brief overlay 광고 시작
@date 2012/12/10
@param videoUrl 동영상 주소
@param Title 동영상 제목
*/
- (void) startAdOverlayWithContentURL:(NSString *)videoUrl title:(NSString *)Title;


/**
 @brief overlay 광고 시작
 @date 2012/12/10
 @param (NSString*)videoUrl 동영상 주소
 @param (NSString*)Title 동영상 제목
 @param (AdHubAdSize)size 배너 사이즈
 */
-(void)startAdOverlayWithContentURL:(NSString *)videoUrl title:(NSString *)Title bannerSize:(AdHubAdSize)size;


/**
@brief user profile 설정
@date 2012/12/10
@param p user profile 객체
@remark 광고소비자의 나이,성별,종교,최미,관심사 설정
*/
- (void) setUserProfile:(AdHubUserProfile *)p;


-(void)setMcc:(NSString *)mcc;


@end
