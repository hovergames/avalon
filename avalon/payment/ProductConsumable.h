#ifndef AVALON_PAYMENT_PRODUCTCONSUMABLE_H
#define AVALON_PAYMENT_PRODUCTCONSUMABLE_H

#include <avalon/payment/Product.h>

namespace avalon {
namespace payment {

class ProductConsumable : public Product
{
public:
    ProductConsumable(const char* const productId, const float quantityPerPurchase);
    virtual ~ProductConsumable();

    virtual void consume() override;
    float getQuantity() const;

protected:
    const float quantityPerPurchase;
};

} // namespace payment
} // namespace avalon

#endif /* AVALON_PAYMENT_PRODUCTCONSUMABLE_H */
