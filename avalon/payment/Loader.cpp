#include <avalon/payment/Loader.h>

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <avalon/payment/Product.h>
#include <avalon/payment/ProductConsumable.h>
#include <avalon/payment/Manager.h>
#include <avalon/utils/platform.h>

namespace avalon {
namespace payment {

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

        if (!config.getSection(sectionName)->count("type")) {
            BOOST_ASSERT_MSG(false, "Product has no type defined");
            continue;
        }
        const char* type = config.getValue(sectionName, "type");

        const char* productId = detectProductId(sectionName);
        if (productId == NULL) {
            BOOST_ASSERT_MSG(false, "Product has no productId set for this platform");
            continue;
        }
        
        Product *product = NULL;
        if (strcmp(type, "non-consumable") == 0) {
            product = new Product(productId);
        } else if (strcmp(type, "consumable") == 0) {
            float quantity = config.getValueAsFloat(sectionName, "quantity");
            BOOST_ASSERT_MSG(quantity > 0, "Quantity missing or set to zero");

            product = new ProductConsumable(productId, quantity);
        } else {
            BOOST_ASSERT_MSG(false, "Couldn't identify product type from ini file");
            continue;
        }

        manager->addProduct(product, sectionName);
    }
}

const char* Loader::detectProductId(const char* section)
{
    auto flavor = avalon::utils::platform::getFlavor();
    flavor[0] = std::toupper(flavor[0]);
    auto prefix = avalon::utils::platform::getName() + flavor + "Id";

    if (!config.getSection(section)->count(prefix.c_str())) {
        return NULL;
    }
    
    const char* productId = config.getValue(section, prefix.c_str());
    if (!productId || strlen(productId) == 0) {
        return NULL;
    }

    return productId;
}

shared_ptr<Manager> Loader::getManager() const
{
    return manager;
}

} // namespace payment
} // namespace avalon
