//
//  PaymentProviderIos.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#include "PaymentProviderIos.h"
#include "PaymentListener.h"
#include <string>
#include "StoreManager.h"
#include "Helpers.h"
#include "cocos2d.h"
#include "Product.h"

PaymentListener* PaymentProviderIos::listener = 0;
std::map<std::string, Product*> PaymentProviderIos::products;

PaymentProviderIos::~PaymentProviderIos()
{
}

Product * PaymentProviderIos::getProductByNativeId(const char *id)
{
    for (std::map<std::string, Product*>::iterator it = products.begin(); it != products.end(); ++it)
    {
        if (strcmp(it->second->nativeId.c_str(), id) == 0)
        {
            return (it->second);
        }
    }
    return NULL;
}

void PaymentProviderIos::setBundleId(const char *bundleId)
{
    this->bundleId = std::string(bundleId);
}

void PaymentProviderIos::addProduct(Product *product)
{
    product->paymentProvider = this;
    products.insert(std::make_pair<std::string, Product*>(product->internalId, product));
}

void PaymentProviderIos::setListener(PaymentListener *paymentListener)
{
    [StoreManager sharedManager].paymentListener = paymentListener;
}

void PaymentProviderIos::init()
{
    [[StoreManager sharedManager] setWasError: NO];
	[StoreManager sharedManager];
	[[StoreManager sharedManager] setBundleID:[NSString stringWithCString:bundleId.c_str() encoding:[NSString defaultCStringEncoding]]];
    [StoreManager sharedManager].paymentProvider = this;

	NSMutableArray *nsaProdList = [[NSMutableArray alloc] init];
	NSString *prodID;

    for (std::map<std::string, Product*>::iterator it = products.begin(); it != products.end(); ++it)
    {
		prodID = [NSString stringWithCString:it->second->nativeId.c_str() encoding:[NSString defaultCStringEncoding]];
        [nsaProdList addObject: prodID];
    }

	NSLog(@"Init %@", nsaProdList);

	[[StoreManager sharedManager] setProductsList:[NSSet setWithArray:nsaProdList]];
	[StoreManager startManager];
}

bool PaymentProviderIos::isPurchased(const char *id)
{
    Product *p = getProductByNativeId(id);
    assert(p && "[PaymentProviderIos::isPurchased] Product not found");
	NSString *productId = [NSString stringWithCString:p->nativeId.c_str() encoding:[NSString defaultCStringEncoding]];
	if([StoreManager isFeaturePurchased:productId])
	{
		return true;
	}

    NSLog(@"Product %@ not purchased", productId);
	return false;
}

void PaymentProviderIos::purchase(const char *id)
{
    if ([StoreManager sharedManager].paymentListener)
    {        
        [StoreManager sharedManager].paymentListener->onTransactionStart();
    }
    
    Product *p = products.find(id)->second;
    assert(p && "[PaymentProviderIos::purchase] product not found");
    NSString *productID = [NSString stringWithCString:p->nativeId.c_str() encoding:[NSString defaultCStringEncoding]];
    NSLog(@"[buyProduct] Trying to purchase: %@", productID);

    [[StoreManager sharedManager] buyFeature:productID];
}

bool PaymentProviderIos::consume(const char *id, int quantity)
{
    NSString *productId = [NSString stringWithCString:products[id]->nativeId.c_str() encoding:[NSString defaultCStringEncoding]];
    return [[StoreManager sharedManager] consumeProduct:productId quantity: quantity];
}

bool PaymentProviderIos::isConsumable(const char *id, int quantity)
{
    NSString *productId = [NSString stringWithCString:products[id]->nativeId.c_str() encoding:[NSString defaultCStringEncoding]];
    return [[StoreManager sharedManager] canConsumeProduct:productId quantity:quantity];
}

void PaymentProviderIos::updateProductsDescription() {
	[[StoreManager sharedManager] updateObjectsDescription];
}


void PaymentProviderIos::restorePurchases() {
    NSLog(@"trying to restore");
    if ([StoreManager sharedManager].paymentListener) {
        [StoreManager sharedManager].paymentListener->onTransactionStart();
    }

    [[StoreManager sharedManager] restorePreviousTransactions];
}

Product * PaymentProviderIos::getProduct(const char *productId)
{
    return products[productId];
}

void PaymentProviderIos::requestProductData()
{
    [[StoreManager sharedManager] requestProductData];
}

bool PaymentProviderIos::wasError() {
    return [[StoreManager sharedManager] wasError];
}

int PaymentProviderIos::getQuantity(const char *id)
{
    return [[StoreManager sharedManager] getQuantity:[NSString stringWithCString:products[id]->nativeId.c_str() encoding:[NSString defaultCStringEncoding]]];
}


int PaymentProviderIos::getPurchaseResult() {
	return [[StoreManager sharedManager] purchaseResult];
}

void PaymentProviderIos::resetPurchaseResult() {
	[[StoreManager sharedManager] setPurchaseResult: 0];
}