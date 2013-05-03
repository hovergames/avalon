#include "Backend.h"

#include <StoreKit/StoreKit.h>
#include "Manager.h"
#include "ManagerDelegate.h"
#include "Product.h"
#include "BackendIos.h"

namespace Avalon {
namespace Payment {

BackendIos* const __getIosBackend()
{
    static BackendIos* instance = [[BackendIos alloc] init];
    return instance;
}

Backend::Backend(const Manager& manager)
: manager(manager)
{
}

Backend::~Backend()
{
}

bool Backend::isInitialized() const
{
    return __getIosBackend()->initialized;
}

void Backend::initialize()
{
    // convert Avalon::Payment::ProductList into NSMutableSet
    NSMutableSet* products = [[[NSMutableSet alloc] init] autorelease];
    for (auto pair : manager.getProducts()) {
        const char* const productId = pair.second->getProductId().c_str();
        [products addObject:[NSString stringWithUTF8String:productId]];
    }

    // configure BackendIos
    __getIosBackend()->initialized = false;
    __getIosBackend()->manager = const_cast<Manager*>(&manager);
    __getIosBackend()->transactionDepth = 0;

    // fetch product details
    SKProductsRequest* request = [[SKProductsRequest alloc] initWithProductIdentifiers:products];
    request.delegate = __getIosBackend();
    [request start];

    // register transcationObserver
    [[SKPaymentQueue defaultQueue] addTransactionObserver:__getIosBackend()];
}

void Backend::purchase(Product* const product)
{
    if (__getIosBackend()->transactionDepth++ == 0) {
        manager.delegate->onTransactionStart(__getIosBackend()->manager);
    }

    NSString* productId = [[[NSString alloc] initWithUTF8String:product->getProductId().c_str()] autorelease];
    SKPayment *payment = [SKPayment paymentWithProductIdentifier:productId];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

bool Backend::isPurchaseReady() const
{
    return [SKPaymentQueue canMakePayments];
}

void Backend::restorePurchases() const
{
    if (__getIosBackend()->transactionDepth++ == 0) {
        manager.delegate->onTransactionStart(__getIosBackend()->manager);
    }

    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

} // namespace Payment
} // namespace Avalon