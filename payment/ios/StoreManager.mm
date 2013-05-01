//
//  StoreManager.m
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#import "StoreManager.h"
#import "StoreKitDelegate.h"
#import "PaymentProviderIos.h"
#include "PaymentListener.h"

@implementation StoreManager

@synthesize purchasableObjects = _purchasableObjects;
@synthesize storeObserver = _storeObserver;
@synthesize bundleID;
@synthesize purchaseResult;
@synthesize wasError;
@synthesize paymentListener;

static NSString *ownServer = nil;

static __weak id<StoreKitDelegate> _delegate;
static StoreManager* _sharedStoreManager;


- (void)dealloc {
	[_purchasableObjects release];
	[_storeObserver release];

	[bundleID release];
    //	[_productsList release];

	[_sharedStoreManager release];
	[super dealloc];
}

#pragma mark Delegates

+ (id)delegate {

    return _delegate;
}

+ (void)setDelegate:(id)newDelegate {

    _delegate = newDelegate;
}

#pragma mark Singleton Methods

+ (StoreManager*)sharedManager
{
	@synchronized(self) {

        if (_sharedStoreManager == nil) {
            _sharedStoreManager = [[self alloc] init];
        }
    }
    return _sharedStoreManager;
}


+ (void)startManager
{
    _sharedStoreManager.purchasableObjects = [[NSMutableArray alloc] init];
    [_sharedStoreManager requestProductData];
    _sharedStoreManager.storeObserver = [[StoreObserver alloc] init];
    [[SKPaymentQueue defaultQueue] addTransactionObserver:_sharedStoreManager.storeObserver];
}

+ (id)allocWithZone:(NSZone *)zone
{
    @synchronized(self) {
        if (_sharedStoreManager == nil) {
            _sharedStoreManager = [super allocWithZone:zone];
            return _sharedStoreManager;  // assignment and return on first allocation
        }
    }

    return nil; //on subsequent allocation attempts return nil
}


- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)retain
{
    return self;
}

- (unsigned)retainCount
{
    return UINT_MAX;  //denotes an object that cannot be released
}

- (void)release
{
    //do nothing
}

- (id)autorelease
{
    return self;
}

#pragma mark Internal functions

- (void) setProductsList:(NSSet *) value {
	if (productsList == nil) {
		productsList = [[NSMutableSet alloc] init];
	}

	[productsList setSet:value];
}


- (void) restorePreviousTransactions
{
	[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

-(void) requestProductData
{
	SKProductsRequest *request= [[SKProductsRequest alloc] initWithProductIdentifiers:productsList];
	request.delegate = self;
	[request start];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	[self.purchasableObjects addObjectsFromArray:response.products];

	for(int i=0;i<[self.purchasableObjects count];i++)
	{
		SKProduct *product = [self.purchasableObjects objectAtIndex:i];
        NSLog(@"Feature: %@, Cost: %f, ID: %@",[product localizedTitle],
		 	  [[product price] doubleValue], [product productIdentifier]);
	}

	for(NSString *invalidProduct in response.invalidProductIdentifiers) {
        [[StoreManager sharedManager] setWasError: YES];
	    NSLog(@"Problem in iTunes connect configuration for product: %@", invalidProduct);
    }

	[request autorelease];

	isProductsAvailable = YES;

	if([_delegate respondsToSelector:@selector(productFetchComplete)])
		[_delegate productFetchComplete];

    [self updateObjectsDescription];
}


// call this function to check if the user has already purchased your feature
+ (BOOL) isFeaturePurchased:(NSString*) featureId
{
	return [[NSUserDefaults standardUserDefaults] boolForKey:featureId];
}

// Call this function to populate your UI
// this function automatically formats the currency based on the user's locale
- (void) updateObjectsDescription
{
	for(int i=0;i<[self.purchasableObjects count];i++)
	{
		SKProduct *product = [self.purchasableObjects objectAtIndex:i];
        const char *productId = [[product productIdentifier] UTF8String];
        cocos2d::CCLog("Update Objects description for %s", productId);
        
        Product *p = paymentProvider->getProductByNativeId(productId);

        assert(p && "updateObjectsDescription -> p is null");
        
		NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
		[numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
		[numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
		[numberFormatter setLocale:product.priceLocale];
		NSString *formattedString = [numberFormatter stringFromNumber:product.price];
		[numberFormatter release];

        NSLog(@"Title: %@", [product localizedTitle]);

        p->localizedDescription = [[product localizedTitle] UTF8String];
        p->price = [[product price] floatValue];
        p->localizedPrice = [formattedString UTF8String];
    }
}

- (void) buyFeature:(NSString*) featureId
{
	if ([SKPaymentQueue canMakePayments])
	{
		NSLog(@"[buyFeature] Trying to buy %@", featureId);
		SKPayment *payment = [SKPayment paymentWithProductIdentifier:featureId];
		[[SKPaymentQueue defaultQueue] addPayment:payment];
	}
	else
	{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"In-App Purchasing disabled", @"")
														message:NSLocalizedString(@"Check your parental control settings and try again later", @"")
													   delegate:self
											  cancelButtonTitle:NSLocalizedString(@"Dismiss", @"")
											  otherButtonTitles: nil];
		[alert show];
		[alert release];
	}
}

- (BOOL) canConsumeProduct:(NSString*) productIdentifier
{
	int count = [[NSUserDefaults standardUserDefaults] integerForKey:productIdentifier];
	return (count > 0);

}

- (int) getQuantity:(NSString*) productIdentifier
{
    return [[NSUserDefaults standardUserDefaults] integerForKey:productIdentifier];
}

- (BOOL) canConsumeProduct:(NSString*) productIdentifier quantity:(int) quantity
{
	int count = [[NSUserDefaults standardUserDefaults] integerForKey:productIdentifier];
	return (count >= quantity);
}

- (BOOL) consumeProduct:(NSString*) productIdentifier quantity:(int) quantity
{
	int count = [[NSUserDefaults standardUserDefaults] integerForKey:productIdentifier];
	if(count < quantity)
	{
		return NO;
	}
	else
	{
        NSLog(@"old count: %d (-%d)", count, quantity);
		count -= quantity;
		[[NSUserDefaults standardUserDefaults] setInteger:count forKey:productIdentifier];
        NSLog(@"new count: %d", count);
		return YES;
	}

}

-(void) enableContentForThisSession: (NSString*) productIdentifier
{
	if([_delegate respondsToSelector:@selector(productPurchased:)])
		[_delegate productPurchased:productIdentifier];
}


#pragma mark In-App purchases callbacks
// In most cases you don't have to touch these methods
-(void) provideContent: (NSString*) productIdentifier
            forReceipt:(NSData*) receiptData
{
	NSLog(@"Provide Content!");
	if(ownServer != nil && SERVER_PRODUCT_MODEL)
	{
		// ping server and get response before serializing the product
		// this is a blocking call to post receipt data to your server
		// it should normally take a couple of seconds on a good 3G connection
		if(![self verifyReceipt:receiptData]) return;
	}

	NSRange range = [productIdentifier rangeOfString: @"." options: NSBackwardsSearch];
	// if (range.location == NSNotFound) NSLog(@"invalid product id");

	NSString *countText = [productIdentifier substringFromIndex:range.location+1];

	int quantityPurchased = [countText intValue];
	NSLog(@"%d Quantity purchased", quantityPurchased);

	if(quantityPurchased != 0)
	{
		int count = [[NSUserDefaults standardUserDefaults] integerForKey:countText];
		count += quantityPurchased;
		[[NSUserDefaults standardUserDefaults] setInteger:count forKey:countText];

        NSLog(@"%d <-- new count", count);
	}
	else
	{
        if (productIdentifier)
        {
            [[NSUserDefaults standardUserDefaults] setBool:YES forKey:productIdentifier];
        }
	}

	[[NSUserDefaults standardUserDefaults] synchronize];

	if([_delegate respondsToSelector:@selector(productPurchased:)])
		[_delegate productPurchased:productIdentifier];
}

- (void) transactionCanceled: (SKPaymentTransaction *)transaction
{
    NSLog(@"User cancelled transaction: %@", [transaction description]);

    if (transaction.error.code != SKErrorPaymentCancelled)
    {
		if(transaction.error.code == SKErrorUnknown) {
            NSLog(@"Unknown Error (%d), product: %@", (int)transaction.error.code, transaction.payment.productIdentifier);
			UIAlertView *failureAlert = [[UIAlertView alloc] initWithTitle :@"In-App-Purchase Error:"
																	message: @"There was an error purchasing this item please try again."
																  delegate : self cancelButtonTitle:@"OK"otherButtonTitles:nil];
			[failureAlert show];
			[failureAlert release];
		}

		if(transaction.error.code == SKErrorClientInvalid) {
            NSLog(@"Client invalid (%d), product: %@", (int)transaction.error.code, transaction.payment.productIdentifier);
			UIAlertView *failureAlert = [[UIAlertView alloc] initWithTitle :@"In-App-Purchase Error:"
																	message: @"There was an error purchasing this item please try again."
																  delegate : self cancelButtonTitle:@"OK"otherButtonTitles:nil];
			[failureAlert show];
			[failureAlert release];
		}

		if(transaction.error.code == SKErrorPaymentInvalid) {
            NSLog(@"Payment invalid (%d), product: %@", (int)transaction.error.code, transaction.payment.productIdentifier);
			UIAlertView *failureAlert = [[UIAlertView alloc] initWithTitle :@"In-App-Purchase Error:"
																	message: @"There was an error purchasing this item please try again."
																  delegate : self cancelButtonTitle:@"OK"otherButtonTitles:nil];
			[failureAlert show];
			[failureAlert release];
		}

		if(transaction.error.code == SKErrorPaymentNotAllowed) {
            NSLog(@"Payment not allowed (%d), product: %@", (int)transaction.error.code, transaction.payment.productIdentifier);
			UIAlertView *failureAlert = [[UIAlertView alloc] initWithTitle :@"In-App-Purchase Error:"
																	message: @"There was an error purchasing this item please try again."
																  delegate : self cancelButtonTitle:@"OK"otherButtonTitles:nil];
			[failureAlert show];
			[failureAlert release];
		}
    }

	if([_delegate respondsToSelector:@selector(transactionCanceled)])
		[_delegate transactionCanceled];

    if (self.paymentListener)
    {
        const char *productId = [transaction.payment.productIdentifier UTF8String];
        self.paymentListener->onPurchaseStateChanged(PaymentProviderIos::products[productId]);
        self.paymentListener->onTransactionStop();
    }
}

- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[transaction.error localizedFailureReason]
													message:[transaction.error localizedRecoverySuggestion]
												   delegate:self
										  cancelButtonTitle:NSLocalizedString(@"Dismiss", @"")
										  otherButtonTitles: nil];
	[alert show];
	[alert release];
}
@end
