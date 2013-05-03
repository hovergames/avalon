#ifndef __Avalon__Payment__ProductConsumable__
#define __Avalon__Payment__ProductConsumable__

#include "Product.h"

namespace Avalon {
namespace Payment {

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

} // namespace Payment
} // namespace Avalon

#endif /* __Avalon__Payment__ProductConsumable__ */