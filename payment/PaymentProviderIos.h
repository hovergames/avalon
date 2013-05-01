//
//  PaymentProviderIos.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#ifndef __Adventures_on_the_farm__PaymentProviderIos__
#define __Adventures_on_the_farm__PaymentProviderIos__

#include "PaymentProvider.h"
#include <map>
#include "Product.h"
#include <string>

/*
 * Example of how to use this payment provider
 * This is not yet suitable for production, its just
 * the first draft based on my code from my moneky code base
 *
 * TODO:
 * --> Add PaymentService class
 * --> Add Produt classes
 * --> Callback based implementation
 *
 * PaymentProviderIos *payment = new PaymentProviderIos();
 * payment->setBundleId("com.intermediaware.motorcycles.");
 * payment->addProduct("com.intermediaware.motorcycles.unlockall");
 * payment->init();
 * 
 */

class PaymentProviderIos : public PaymentProvider
{
public:
    static PaymentListener *listener;

    std::string bundleId;
    static std::map<std::string, Product*> products;

    virtual ~PaymentProviderIos();
    
    virtual void init();
    virtual void setBundleId(const char *bundleId);

    virtual void addProduct(Product *product);
    Product * getProduct(const char *productId);
    Product * getProductByNativeId(const char *id);

    virtual void purchase(const char *id);
    virtual bool consume(const char *id, int quantity);
    virtual void restorePurchases();

    virtual bool isConsumable(const char *id, int quantity);
    virtual bool isPurchased(const char *id);
    virtual int getQuantity(const char *id);

    virtual void setListener(PaymentListener *paymentListener);

    virtual void requestProductData();
    virtual void updateProductsDescription();

    virtual bool wasError();

    virtual void resetPurchaseResult();
    virtual int getPurchaseResult();
};

#endif /* defined(__Adventures_on_the_farm__PaymentProviderIos__) */
