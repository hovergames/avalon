#include <avalon/payment/Manager.h>

#include <typeinfo>
#include <boost/assert.hpp>
#include <boost/cast.hpp>
#include <avalon/payment/Product.h>
#include <avalon/payment/ProductConsumable.h>

using boost::polymorphic_downcast;
using std::string;
using std::make_pair;

namespace avalon {
namespace payment {

Manager::Manager()
: ignoreUnusedConsumableQuantities(false)
, delegate()
, backend(*this)
, products()
, productIdAliases()
, started(false)
{
}

Manager::~Manager()
{
    for (auto& pair : products) {
        auto& product = pair.second;

        if (ignoreUnusedConsumableQuantities) {
            auto consumable = dynamic_cast<ProductConsumable* const>(product);
            if (consumable) {
                consumable->consume();
            }
        }

        delete product;
    }

    products.clear();
    productIdAliases.clear();
}

void Manager::addProduct(Product* const product)
{
    if (backend.isInitialized()) {
        BOOST_ASSERT_MSG(false, "backend already initialized");
        return;
    }

    if (!product) {
        BOOST_ASSERT_MSG(false, "product must be given");
        return;
    }
    BOOST_ASSERT_MSG(!hasProduct(product->getProductId().c_str()), "productId already in use");

    products.insert(make_pair(
        product->getProductId(),
        product
    ));
    product->manager = this;
}

void Manager::addProduct(Product* const product, const char* const alias)
{
    BOOST_ASSERT_MSG(!hasProduct(alias), "given alias already in use");

    addProduct(product);
    productIdAliases.insert(make_pair(
        string(alias),
        product->getProductId()
    ));
}

const ProductList& Manager::getProducts() const
{
    return products;
}

Product* Manager::getProduct(const char* const productIdOrAlias) const
{
    auto productId = string(productIdOrAlias);
    auto product = products.find(productId);
    if (product != products.end()) {
        return product->second;
    }

    if (productIdAliases.count(productId) > 0) {
        auto aliasedId = productIdAliases.at(productId);
        auto aliasedProduct = products.find(aliasedId);
        if (aliasedProduct != products.end()) {
            return aliasedProduct->second;
        }
    }

    BOOST_ASSERT_MSG(false, "invalid productId or alias given");
    return NULL;
}

ProductConsumable* Manager::getProductConsumable(const char* const productIdOrAlias) const
{
    return polymorphic_downcast<ProductConsumable* const>(getProduct(productIdOrAlias));
}

bool Manager::hasProduct(const char* const productIdOrAlias) const
{
    auto productId = string(productIdOrAlias);
    
    if (products.count(productId) > 0) {
        return true;
    }

    if (productIdAliases.count(productId) == 0) {
        return false;
    }

    auto aliasId = productIdAliases.at(productId);
    return (products.count(aliasId) > 0);
}

void Manager::purchase(const char* const productIdOrAlias)
{
    if (!isPurchaseReady()) {
        BOOST_ASSERT_MSG(false, "backend service not started yet");
        return;
    }

    auto product = getProduct(productIdOrAlias);
    if (product) {
        backend.purchase(product);
    }
}

void Manager::startService()
{
    if (isStarted()) {
        BOOST_ASSERT_MSG(false, "service already started");
        return;
    }

    if (!delegate) {
        throw new std::runtime_error("payment delegate must be set!");
    }

    backend.initialize();
    started = true;
}

void Manager::stopService()
{
    backend.shutdown();
    started = false;
}

bool Manager::isStarted() const
{
    return started;
}

bool Manager::isPurchaseReady() const
{
    return (delegate && backend.isInitialized() && backend.isPurchaseReady());
}

void Manager::restorePurchases() const
{
    if (!isPurchaseReady()) {
        BOOST_ASSERT_MSG(false, "backend service not started yet");
        return;
    }

    backend.restorePurchases();
}

Backend& Manager::getBackend()
{
    return backend;
}

} // namespace payment
} // namespace avalon
