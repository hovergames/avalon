//
//  StoreObserver.m
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#import "StoreObserver.h"
#import "StoreManager.h"
#include "PaymentProviderIos.h"
#include "PaymentListener.h"

@implementation StoreObserver
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	for (SKPaymentTransaction *transaction in transactions)
	{
		switch (transaction.transactionState)
		{
			case SKPaymentTransactionStatePurchasing:
                NSLog(@"State: Purchasing");
				break;

			case SKPaymentTransactionStatePurchased:
                NSLog(@"State: Purchases");
                if ([StoreManager sharedManager].paymentListener) {
                    [StoreManager sharedManager].paymentListener->onTransactionStart();
                }

				[[StoreManager sharedManager] setPurchaseResult: 2];
                [self completeTransaction:transaction];
                break;

            case SKPaymentTransactionStateFailed:
                NSLog(@"State: Failed");
                if ([StoreManager sharedManager].paymentListener) {
                    [StoreManager sharedManager].paymentListener->onTransactionStart();
                }


				[[StoreManager sharedManager] setPurchaseResult: 3];
                [self failedTransaction:transaction];
                break;

            case SKPaymentTransactionStateRestored:
                NSLog(@"State: Restored");
                if ([StoreManager sharedManager].paymentListener)
                {
                    [StoreManager sharedManager].paymentListener->onTransactionStart();
                }


				[[StoreManager sharedManager] setPurchaseResult: 4];
                [self restoreTransaction:transaction];

            default:
                NSLog(@"unhandeled");
                break;
		}
	}
}

- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
	NSLog(@"%@", transaction);
	switch (transaction.error.code)
	{
		case SKErrorPaymentCancelled:
            NSLog(@"SKErrorPaymentCancelled");
            break;

		case SKErrorUnknown:
            NSLog(@"SKErrorUnknown %@ | %@", transaction.error.localizedDescription, transaction.error.localizedFailureReason );
            break;

		case SKErrorClientInvalid:
            NSLog(@"SKErrorClientInvalid");
            break;

		case SKErrorPaymentInvalid:
            NSLog(@"SKErrorPaymentInvalid");
            break;

		case SKErrorPaymentNotAllowed:
			NSLog(@"SKErrorPaymentNotAllowed");
            break;

		default:
            NSLog(@"## MISSING ERROR CODE TRANSCATION");
			break;
	}

    if ([StoreManager sharedManager].paymentListener)
    {
        const char *productId = [transaction.payment.productIdentifier UTF8String];
        Product *p = [StoreManager sharedManager].paymentProvider->getProductByNativeId(productId);
        if (p)
        {
            [StoreManager sharedManager].paymentListener->onPurchaseStateChanged(p);
            [StoreManager sharedManager].paymentListener->onTransactionStop();
        }
    }
}


- (void) completeTransaction: (SKPaymentTransaction *)transaction
{
	[[StoreManager sharedManager] provideContent:transaction.payment.productIdentifier
                                        forReceipt:transaction.transactionReceipt];

	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];

	NSLog(@"IAP Purchase completed: %@", transaction.payment.productIdentifier);

    if ([StoreManager sharedManager].paymentListener)
    {
        const char *productId = [transaction.payment.productIdentifier UTF8String];
        [StoreManager sharedManager].paymentListener->onPurchaseStateChanged([StoreManager sharedManager].paymentProvider->getProductByNativeId(productId));
        [StoreManager sharedManager].paymentListener->onTransactionStop();
    }
}

- (void) restoreTransaction: (SKPaymentTransaction *)transaction
{
    [[StoreManager sharedManager] provideContent: transaction.originalTransaction.payment.productIdentifier
                                        forReceipt:transaction.transactionReceipt];

    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];

    NSLog(@"IAP Restore Purchases completed");

    if ([StoreManager sharedManager].paymentListener)
    {
        const char *productId = [transaction.originalTransaction.payment.productIdentifier UTF8String];
        [StoreManager sharedManager].paymentListener->onPurchaseStateChanged([StoreManager sharedManager].paymentProvider->getProductByNativeId(productId));
        [StoreManager sharedManager].paymentListener->onTransactionStop();
    }
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    NSLog(@"paymentQueueRestoreCompletedTransactionsFinished");
    if ([StoreManager sharedManager].paymentListener)
    {
        [StoreManager sharedManager].paymentListener->onTransactionStop();
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    if ([StoreManager sharedManager].paymentListener)
    {
        [StoreManager sharedManager].paymentListener->onTransactionStop();
    }

    NSLog(@"Error: %@", error);
}

@end
