//
//  SADBannerViewDelegate.h
//  SADAdHub
//
//  Created by dsjang on 12. 8. 28..
//  Copyright (c) 2012년 dsjang. All rights reserved.
//

#import <Foundation/Foundation.h>

@class SADPlayerConnection;
@protocol AdHubPlayerDelegate <NSObject>

@optional


/**
@brief 동영상 플레이어에서 에러가 발생한 경우 호출됨
@date 2012/12/10
*/
- (void)adHubPlayerError;

/**
@brief 동영상 플레이어가 종료된 경우 호출 됨
@date 2012/12/10
*/
- (void)adHubPlayerClose;
@end

