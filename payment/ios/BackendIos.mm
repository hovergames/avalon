#import "BackendIos.h"

#include "Product.h"
#include "ManagerDelegate.h"

@implementation BackendIos

- (void)startProductRequest
{
    SKProductsRequest* request = [[SKProductsRequest alloc] initWithProductIdentifiers:products];
    request.delegate = self;
    [request start];
}

- (void)startTransactionObserver
{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}

- (void)purchase:(NSString *)productId
{
    SKPayment *payment = [SKPayment paymentWithProductIdentifier:productId];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (void)restoreCompleteTransactions
{
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

#pragma mark -
#pragma mark SKProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSNumberFormatter* numberFormatter = [[[NSNumberFormatter alloc] init] autorelease];
    [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];

    for (SKProduct* skProduct in response.products) {
        const char* productId = [[skProduct productIdentifier] cStringUsingEncoding:NSASCIIStringEncoding];

        Avalon::Payment::Product* avProduct = manager->getProduct(productId);
        avProduct->price = [[skProduct price] floatValue];
        avProduct->localizedName = [[skProduct localizedTitle] UTF8String];
        avProduct->localizedDescription = [[skProduct localizedDescription] UTF8String];

		[numberFormatter setLocale:skProduct.priceLocale];
        avProduct->localizedPrice = [[numberFormatter stringFromNumber:skProduct.price] UTF8String];
    }

    for (NSString* productId in response.invalidProductIdentifiers) {
        NSLog(@"[Payment] productsRequest: Invalid productId: %@", productId);
    }

    initialized = true;
    manager->delegate->onServiceStarted(manager);
}

#pragma mark -
#pragma mark SKPaymentTransactionObserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	for (SKPaymentTransaction *transaction in transactions) {
		switch (transaction.transactionState) {
			case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;

            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;

            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
                break;
                
            default:
                NSLog(@"[Payment] paymentQueue: UNHANDELED");
                break;
		}
	}
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    manager->delegate->onTransactionEnd(manager);
    manager->delegate->onRestoreSucceed(manager);
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    NSLog(@"[Payment] restoreCompletedTransactions failed: %@", error.localizedDescription);

    manager->delegate->onTransactionEnd(manager);
    manager->delegate->onRestoreFail(manager);
}

#pragma mark -
#pragma mark Processing

- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    manager->delegate->onTransactionEnd(manager);

    const char* productId = [transaction.payment.productIdentifier cStringUsingEncoding:NSASCIIStringEncoding];
    manager->delegate->onPurchaseSucceed(manager, manager->getProduct(productId));

	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
    manager->delegate->onTransactionEnd(manager);

    const char* productId = [transaction.originalTransaction.payment.productIdentifier cStringUsingEncoding:NSASCIIStringEncoding];
    manager->delegate->onPurchaseSucceed(manager, manager->getProduct(productId));

    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
	switch (transaction.error.code) {
		case SKErrorPaymentCancelled:
            NSLog(@"[Payment] failedTransaction: SKErrorPaymentCancelled");
            break;

		case SKErrorUnknown:
            NSLog(@"[Payment] failedTransaction: SKErrorUnknown: %@ | %@", transaction.error.localizedDescription, transaction.error.localizedFailureReason );
            break;

		case SKErrorClientInvalid:
            NSLog(@"[Payment] failedTransaction: SKErrorClientInvalid");
            break;

		case SKErrorPaymentInvalid:
            NSLog(@"[Payment] failedTransaction: SKErrorPaymentInvalid");
            break;

		case SKErrorPaymentNotAllowed:
			NSLog(@"[Payment] failedTransaction: SKErrorPaymentNotAllowed");
            break;

		default:
            NSLog(@"[Payment] failedTransaction: UNHANDELED");
			break;
	}

    manager->delegate->onTransactionEnd(manager);
    manager->delegate->onPurchaseFail(manager);
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

@end
