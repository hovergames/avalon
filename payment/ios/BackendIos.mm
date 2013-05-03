#import "BackendIos.h"

#include "Product.h"
#include "Manager.h"
#include "ManagerDelegate.h"

@implementation BackendIos

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

            case SKPaymentTransactionStatePurchasing:
                // this state is known, fine and can be ignored
                break;

            default:
                NSLog(@"[Payment] paymentQueue: UNHANDELED: %d", transaction.transactionState);
                break;
		}
	}
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    if (--transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
    manager->delegate->onRestoreSucceed(manager);
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    NSLog(@"[Payment] restoreCompletedTransactions failed: %@", error.localizedDescription);

    if (--transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
    manager->delegate->onRestoreFail(manager);
}

#pragma mark -
#pragma mark Helper

- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    const char* productId = [transaction.payment.productIdentifier cStringUsingEncoding:NSASCIIStringEncoding];
    Avalon::Payment::Product* product = manager->getProduct(productId);
    product->onHasBeenPurchased();

    if (--transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
    manager->delegate->onPurchaseSucceed(manager, product);

	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
    const char* productId = [transaction.originalTransaction.payment.productIdentifier cStringUsingEncoding:NSASCIIStringEncoding];
    Avalon::Payment::Product* product = manager->getProduct(productId);
    product->onHasBeenPurchased();

    if (--transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
    manager->delegate->onPurchaseSucceed(manager, product);

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
            NSLog(@"[Payment] failedTransaction: UNHANDELED: %d", transaction.error.code);
			break;
	}

    if (--transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
    manager->delegate->onPurchaseFail(manager);
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

@end
