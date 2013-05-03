#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

namespace Avalon { namespace Payment { class Manager; } }

@interface BackendIos : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    @public bool initialized;
    @public Avalon::Payment::Manager* manager;
    @public int transactionDepth;
}

#pragma mark -
#pragma mark SKProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;

#pragma mark -
#pragma mark SKPaymentTransactionObserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue;
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error;

#pragma mark -
#pragma mark Helper

- (void)failedTransaction:(SKPaymentTransaction *)transaction;
- (void)completeTransaction:(SKPaymentTransaction *)transaction;
- (void)restoreTransaction:(SKPaymentTransaction *)transaction;

@end