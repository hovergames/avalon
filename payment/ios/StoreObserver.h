//
//  StoreObserver.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#import <Foundation/Foundation.h>
#import "StoreKit/StoreKit.h"

@interface StoreObserver : NSObject<SKPaymentTransactionObserver> {}
- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void) failedTransaction:(SKPaymentTransaction *)transaction;
- (void) completeTransaction:(SKPaymentTransaction *)transaction;
- (void) restoreTransaction:(SKPaymentTransaction *)transaction;

- (void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue;
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error;
@end
