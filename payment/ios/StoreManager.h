//
//  StoreManager.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#import <Foundation/Foundation.h>
#import "StoreKit/StoreKit.h"
#import "StoreObserver.h"
#import "PaymentListener.h"
#import "PaymentProviderIos.h"

@interface StoreManager : NSObject<SKProductsRequestDelegate> {
	NSMutableArray *_purchasableObjects;
	StoreObserver *_storeObserver;

	NSMutableSet *productsList;
	NSString *bundleID;

	BOOL isProductsAvailable;
    BOOL wasError;
	int purchaseResult;

    @public
    PaymentListener *paymentListener;
    PaymentProviderIos *paymentProvider;
}

@property (nonatomic, retain) NSMutableArray *purchasableObjects;
@property (nonatomic, retain) StoreObserver *storeObserver;
@property (nonatomic, retain) NSString *bundleID;
@property (readwrite, assign) BOOL wasError;
@property (readwrite, assign) int purchaseResult;
@property (readwrite, assign) PaymentListener *paymentListener;
@property (readwrite, assign) PaymentProviderIos *paymentProvider;
@property (copy) NSSet *productsList;

+ (StoreManager*)sharedManager;
+ (BOOL) isFeaturePurchased:(NSString*) featureId;
- (void) buyFeature:(NSString*) featureId;
- (void) updateObjectsDescription;
- (void) restorePreviousTransactions;
- (void) setProductsList:(NSSet*) value;
- (BOOL) canConsumeProduct:(NSString*) productIdentifier quantity:(int) quantity;
- (BOOL) consumeProduct:(NSString*) productIdentifier quantity:(int) quantity;
- (int) getQuantity:(NSString*) productIdentifier;

+(id)delegate;
+(void)setDelegate:(id)newDelegate;

@end
