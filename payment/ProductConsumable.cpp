//
//  ProductConsumable.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#include "ProductConsumable.h"

bool ProductConsumable::isConsumable(int quantity)
{
    return paymentProvider->isConsumable(nativeId.c_str(), quantity);
}

bool ProductConsumable::consume(int quantity)
{
    return paymentProvider->consume(nativeId.c_str(), quantity);
}

int ProductConsumable::getQuantity()
{
    return paymentProvider->getQuantity(nativeId.c_str());
}

int ProductConsumable::getVirtualQuantity()
{
    if (virtualQuantity == 0)
    {
        assert(!"THIS IS NOT YET IMPLEMENTED!!!!");
        // TODO: Extract number from product id
    }
}
