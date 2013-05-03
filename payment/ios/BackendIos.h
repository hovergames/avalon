#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#include "Manager.h"

@interface BackendIos : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    @public NSMutableSet* products;
    @public bool initialized;
    @public Avalon::Payment::Manager* manager;
}

- (void)startProductRequest;
- (void)startTransactionObserver;
- (void)purchase:(NSString *)productId;
- (void)restoreCompleteTransactions;

#pragma mark -
#pragma mark SKProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;

#pragma mark -
#pragma mark SKPaymentTransactionObserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue;
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error;

#pragma mark -
#pragma mark Processing

- (void)failedTransaction:(SKPaymentTransaction *)transaction;
- (void)completeTransaction:(SKPaymentTransaction *)transaction;
- (void)restoreTransaction:(SKPaymentTransaction *)transaction;

@end