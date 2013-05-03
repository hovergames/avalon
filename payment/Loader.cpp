#include "Loader.h"

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include "Product.h"
#include "ProductConsumable.h"
#include "Manager.h"

namespace Avalon {
namespace Payment {

shared_ptr<Manager> Loader::globalManager;

Loader::Loader(const char* iniFile)
: manager(new Manager())
, config()
{
    config.loadFile(iniFile);

    for (auto section : *config.getSections()) {
        const char* sectionName = section.first.c_str();

        if (strcmp(sectionName, "general") == 0) {
            continue;
        }

        const char* type = config.getValue(sectionName, "type");
        const char* productId = detectProductId(sectionName);
        Product *product = NULL;

        if (strcmp(type, "non-consumable") == 0) {
            product = new Product(productId);
        } else if (strcmp(type, "consumable") == 0) {
            float quantity = config.getValueAsFloat(sectionName, "quantity");
            BOOST_ASSERT_MSG(quantity > 0, "Quantity missing or set to zero");
            
            product = new ProductConsumable(productId, quantity);
        } else {
            BOOST_ASSERT_MSG(false, "Couldn't identify product type from ini file");
        }

        manager->addProduct(product, sectionName);
    }
}

const char* Loader::detectProductId(const char* section)
{
    const char* productId = NULL;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    productId = config.getValue(section, "iosId");
    if (!productId || strlen(productId) == 0) {
        BOOST_ASSERT_MSG(false, "iosId for product missing");
    }

#else

    BOOST_STATIC_ASSERT_MSG(false, "unsupported platform");

#endif

    return productId;
}

const shared_ptr<Manager>& Loader::getManager() const
{
    return manager;
}


} // namespace Payment
} // namespace Avalon