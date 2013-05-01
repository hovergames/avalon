//
//  Product.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#include "Product.h"
#include <cassert>
#include "cocos2d.h"

void Product::setId(const char *id)
{
    internalId = id;
}

void Product::setNativeId(const char *id)
{
    nativeId = id;
}

bool Product::isPurchased()
{
    return paymentProvider->isPurchased(nativeId.c_str());
}

void Product::purchase()
{
    assert(paymentProvider && "paymentProvider not set");
    assert(!nativeId.empty() && "nativeId of Product not set!");
    paymentProvider->purchase(internalId.c_str());
}

const char * Product::getLocalizedName()
{
    return localizedName.c_str();
}

float Product::getPrice()
{
    return price;
}
