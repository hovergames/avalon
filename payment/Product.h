//
//  Product.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#ifndef __Adventures_on_the_farm__Product__
#define __Adventures_on_the_farm__Product__

#include <string>
#include "PaymentProvider.h"
#include "cocos2d.h"

class Product
{
public:
    PaymentProvider *paymentProvider;

    std::string internalId;
    std::string nativeId;

    std::string localizedName;
    std::string localizedDescription;
    std::string localizedPrice;

    float price;

    Product() {}
    virtual ~Product() {}

    virtual void setId(const char *id);
    virtual void setNativeId(const char *id);

    virtual void purchase();

    virtual bool isPurchased();
    virtual const char * getLocalizedName();
    virtual float getPrice();
};

#endif /* defined(__Adventures_on_the_farm__Product__) */
