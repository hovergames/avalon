//
//  PaymentListener.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#ifndef Adventures_on_the_farm_PaymentListener_h
#define Adventures_on_the_farm_PaymentListener_h

class Product;

class PaymentListener
{
public:
    virtual void onPurchaseStateChanged(Product *p) = 0;
    virtual void onTransactionStart() = 0;
    virtual void onTransactionStop() = 0;
};


#endif
