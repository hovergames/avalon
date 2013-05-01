//
//  ProductNonConsumable.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#ifndef __Adventures_on_the_farm__ProductConsumable__
#define __Adventures_on_the_farm__ProductConsumable__

#include "Product.h"

class ProductConsumable : public Product
{
public:
    int virtualQuantity;

    ProductConsumable() : virtualQuantity(0) {}

    virtual ~ProductConsumable() {}

    virtual bool consume(int quantity);

    virtual bool isConsumable(int quantity);
    virtual int getQuantity();
    virtual int getVirtualQuantity();
};

#endif /* defined(__Adventures_on_the_farm__ProductConsumable__) */
