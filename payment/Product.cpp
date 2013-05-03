#include "Product.h"

#include "Manager.h"
#include <boost/assert.hpp>

namespace Avalon {
namespace Payment {

Product::Product(const char* const productId)
: price(0)
, localizedPrice()
, localizedName()
, localizedDescription()
, purchasedCounter(0)
, manager(NULL)
, productId(std::string(productId))
{
}

Product::~Product()
{
}

const std::string& Product::getProductId() const
{
    return productId;
}

bool Product::canBePurchased() const
{
    if (!manager || !manager->isPurchaseReady()) {
        return false;
    }

    return true;
}

void Product::purchase()
{
    BOOST_ASSERT_MSG(manager, "service has to be set");
    
    manager->purchase(this);
}

void Product::onHasBeenPurchased()
{
    ++purchasedCounter;
}

bool Product::hasBeenPurchased() const
{
    return (purchasedCounter > 0);
}

void Product::consume()
{
}

} // namespace Payment
} // namespace Avalon