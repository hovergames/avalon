//
//  AdHubUserProfile.h
//  SADAdHub
//
//  Created by gpmobile on 12. 12. 3..
//  Copyright (c) 2012년 dsjang. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AdHubUserProfile : NSObject {
    NSString *gender;
    int age;
    NSString *interest;
    NSString *maritalstatus;
    NSString *religion;
    
    NSMutableString *rtv;
    NSMutableArray *dInterest;
}

/**
@brief female 상수
@date 2012/12/10
@return @"f" 반환
*/
+(NSString*) FEMALE;

/**
@brief male 상수
@date 2012/12/10
@return @"m" 반환
*/
+(NSString*) MALE;

/**
@brief single 상수
@date 2012/12/10
@return @"single" 반환
*/
+(NSString*) SINGLE;

/**
@brief married 상수
@date 2012/12/10
@return @"married" 반환
*/
+(NSString*) MARRIED;


/**
@brief Buddhism 상수
@date 2012/12/10
@return @"Buddhism" 반환
*/
+(NSString*) BUDDHISM;


/**
@brief Christianity 상수
@date 2012/12/10
@return @"Christianity" 반환
*/
+(NSString*) CHRISTIANITY;


/**
@brief Islam 상수
@date 2012/12/10
@return @"Islam" 반환
*/
+(NSString*) ISLAM;

/**
@brief Judaism 상수
@date 2012/12/10
@return @"Judaism" 반환
*/
+(NSString*) JUDAISM;

/**
@brief Hinduism 상수
@date 2012/12/10
@return @"Hinduism" 반환
*/
+(NSString*) HINDUISM;

/**
@brief Unaffiliated 상수
@date 2012/12/10
@return @"Unaffiliated" 반환
*/
+(NSString*) UNAFFILIATED;


/**
@brief Others 상수
@date 2012/12/10
@return @"Others" 반환
*/
+(NSString*) OTHERS;

/**
@brief 사용자의 성별을 설정
@date 2012/12/10
@parameter g 성별, @"m" 또는 @"f"
@remark m, f 이외의 다른 값이 설정되면 UNKNOWN으로 설정
*/
-(void) setGender:(NSString*)g;


/**
@brief 사용자의 결혼 여부를 설정
@date 2012/12/10
@parameter m 결혼 여부, @"single" 또는 @"married"
@remark single, married 이외의 다른 값이 설정되면 UNKNOWN으로 설정
*/
-(void) setMaritalstatus:(NSString*)m;

/**
@brief 사용자의 종교를 설정
@date 2012/12/10
@parameter r 종교
@remark Buddhism, Christianity, Islam, Judaism, Hinduism, Unaffiliated 이외의 값이 입력되면Others로 처리됨
*/
-(void) setReligion:(NSString*)r;

/**
@brief 사용자의 나이를 설정
@date 2012/12/10
@parameter a 나이, 정수 값
@remark 음수값이 입력되면 0로 100 이상의 값이 입력되면 99로 처리됨
*/
-(void) setAge:(int)a;

/**
@brief 사용자의 관심사를 설정
@date 2012/12/10
@parameter r 종교
*/
-(void)setInterests:(NSString*)i;


-(NSString *) getQuery;


@end
