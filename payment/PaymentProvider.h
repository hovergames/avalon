//
//  PaymentProvider.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#ifndef __Adventures_on_the_farm__PaymentProvider__
#define __Adventures_on_the_farm__PaymentProvider__

class Product;
class PaymentListener;

class PaymentProvider
{
public:
    PaymentProvider();
    virtual ~PaymentProvider();

    virtual void init() = 0;

    virtual void addProduct(Product *product) = 0;
    virtual Product * getProduct(const char *productId) = 0;

    virtual void purchase(const char *id) = 0;
    virtual bool consume(const char *id, int quantity) = 0;
    virtual void restorePurchases() = 0;

    virtual bool isPurchased(const char *id) = 0;
    virtual bool isConsumable(const char *id, int quantity) = 0;

    virtual int getQuantity(const char *id) = 0;

    virtual void setListener(PaymentListener *paymentListener) = 0;
};

#endif /* defined(__Adventures_on_the_farm__PaymentProvider__) */
