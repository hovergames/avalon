//
//  StoreKitDelegate.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#import <Foundation/Foundation.h>

#define SERVER_PRODUCT_MODEL 0

@protocol StoreKitDelegate <NSObject>
@optional
- (void)productFetchComplete;
- (void)productPurchased:(NSString *)productId;
- (void)transactionCanceled;
// as a matter of UX, don't show a "User Canceled transaction" alert view here
// use this only to "enable/disable your UI or hide your activity indicator view etc.,
@end