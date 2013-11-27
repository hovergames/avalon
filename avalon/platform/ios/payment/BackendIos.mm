#import <avalon/platform/ios/payment/BackendIos.h>

#include <avalon/payment/Product.h>
#include <avalon/payment/Manager.h>
#include <avalon/payment/ManagerDelegate.h>

@implementation BackendIos

#pragma mark -
#pragma mark SKProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    if (!manager) {
        return;
    }

    NSNumberFormatter* numberFormatter = [[[NSNumberFormatter alloc] init] autorelease];
    [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];

    for (SKProduct* skProduct in response.products) {
        const char* productId = [[skProduct productIdentifier] cStringUsingEncoding:NSASCIIStringEncoding];
        avalon::payment::Product* avProduct = manager->getProduct(productId);
        if (avProduct == NULL) {
            NSLog(@"[Payment] productsRequest: Product not found on our side - productId: %s", productId);
            continue;
        }
        
        avProduct->price = [[skProduct price] floatValue];
        [numberFormatter setLocale:skProduct.priceLocale];
        avProduct->localizedPrice = [[numberFormatter stringFromNumber:skProduct.price] UTF8String];

        // I once got rejected because the app has crashed here. Strange, huh?
        // Afer a little investigation it's rather simple. You _DON'T_ receive
        // the name and description for the product if (!!) apple has rejected
        // your product.
        //
        // In my case the guy responsible for my review simply had a small
        // question on how to use the consumables in the game. But to get
        // in contact we me he had to flag something in the app with the
        // "Developer Action Needed" stamp. Guess what he has flagged. Right.
        // All consumables and -- after he read me explanation -- he tried the
        // app again and now it crashes all the time. Rejected. Weee. I love it.
        // -- Michael (Crashlytics ID: 519409611610bcbef3330d08)
        NSString* localizedName = [skProduct localizedTitle];
        if (localizedName != NULL) {
            avProduct->localizedName = [localizedName UTF8String];
        }
        NSString* localizedDescription = [skProduct localizedDescription];
        if (localizedDescription != NULL) {
            avProduct->localizedDescription = [localizedDescription UTF8String];
        }
    }

    for (NSString* productId in response.invalidProductIdentifiers) {
        NSLog(@"[Payment] productsRequest: Product not found on apple side - productId: %@", productId);
    }

    if (manager->delegate) {
        manager->delegate->onServiceStarted(manager);
    }
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
                NSLog(@"[Payment] paymentQueue: UNHANDELED: %ld", (long)transaction.transactionState);
                break;
		}
	}
}

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    if (manager && manager->delegate) {
        manager->delegate->onRestoreSucceed(manager);
    }

    transactionDepth = std::max(0, --transactionDepth);
    if (manager && manager->delegate && transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    if (error.code != SKErrorPaymentCancelled) {
        NSLog(@"[Payment] restoreCompletedTransactions failed: %@", error.localizedDescription);

        if (manager && manager->delegate) {
            manager->delegate->onRestoreFail(manager);
        }
    }

    transactionDepth = std::max(0, --transactionDepth);
    if (manager && manager->delegate && transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
}

#pragma mark -
#pragma mark Helper

- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    // it's important to NOT CALL finishTransaction in this case! because we
    // were unable to process this transaction in the user application. apple
    // will try to deliver this transaction again.
    if (!manager) {
        transactionDepth = std::max(0, --transactionDepth);
        NSLog(@"[Payment] completeTransaction failed: no manager set");
        return;
    }

    const char* productId = [transaction.payment.productIdentifier cStringUsingEncoding:NSASCIIStringEncoding];
    avalon::payment::Product* product = manager->getProduct(productId);
    if (product) {
        product->onHasBeenPurchased();
    } else {
        NSLog(@"[Payment] completeTransaction failed: invalid productId: %s", productId);
    }

    // roughly the same reason as the return above. this is a real transaction and
    // the product was valid a few moments ago. seems to be a bug in the
    // application? nevermind. it's important to try this transaction again
    // and NOT to discard it!
    if (product) {
        [[SKPaymentQueue defaultQueue] finishTransaction: transaction];

        if (manager->delegate) {
            manager->delegate->onPurchaseSucceed(manager, product);
        }
    }

    transactionDepth = std::max(0, --transactionDepth);
    if (manager->delegate && transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
    // we can return early in this case. and we're allowed to finish this
    // transaction too. why? because it's just a restoreTransaction and not
    // a important purchase transaction that must reach the users application
    // as in completeTransaction().
    if (!manager) {
        transactionDepth = std::max(0, --transactionDepth);
        [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
        return;
    }

    const char* productId = [transaction.originalTransaction.payment.productIdentifier cStringUsingEncoding:NSASCIIStringEncoding];
    avalon::payment::Product* product = manager->getProduct(productId);
    if (product) {
        product->onHasBeenPurchased();
    } else {
        NSLog(@"[Payment] restoreTransaction invalid productId: %s", productId);
    }

    if (manager->delegate && product) {
        manager->delegate->onPurchaseSucceed(manager, product);
    }

    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
    bool reportFailure = true;
	switch (transaction.error.code) {
		case SKErrorPaymentCancelled:
            NSLog(@"[Payment] failedTransaction: SKErrorPaymentCancelled");
            reportFailure = false;
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
            NSLog(@"[Payment] failedTransaction: UNHANDELED: %ld", (long)transaction.error.code);
			break;
	}

    if (manager && manager->delegate && reportFailure) {
        manager->delegate->onPurchaseFail(manager);
    }

    transactionDepth = std::max(0, --transactionDepth);
    if (manager && manager->delegate && transactionDepth == 0) {
        manager->delegate->onTransactionEnd(manager);
    }

    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

@end
