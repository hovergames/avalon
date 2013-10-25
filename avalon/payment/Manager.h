#ifndef AVALON_PAYMENT_MANAGER_H
#define AVALON_PAYMENT_MANAGER_H

#include <string>
#include <map>
#include <boost/utility.hpp>
#include <avalon/payment/Backend.h>

namespace avalon {
namespace payment {

class ManagerDelegate;
class Product;
class ProductConsumable;
typedef std::map<const std::string, Product* const> ProductList;

/**
 * Main actor for the whole payment handling and also the mose user facing part.
 *
 * - Keeps track of all available products
 * - Instantiates and holds the native payment::Backend
 * - Can store products under a second name (alias)
 * - Simple interface to start product purchase with only a string
 *
 * @warning It's a good idea to use payment::Loader to instaniate and configure
 *          this class!
 */
class Manager : boost::noncopyable
{
public:
    /**
     * Controls wether all payment::ProductConsumable with quantity > 0 should
     * be consumed in ~Manager() to avoid/ignore the otherwise triggered error.
     */
    bool ignoreUnusedConsumableQuantities;
    ManagerDelegate* delegate;

    Manager();
    ~Manager();

    void addProduct(Product* const product);
    void addProduct(Product* const product, const char* const alias);

    const ProductList& getProducts() const;
    Product* getProduct(const char* const productIdOrAlias) const;
    ProductConsumable* getProductConsumable(const char* const productIdOrAlias) const;
    bool hasProduct(const char* const productIdOrAlias) const;

    void purchase(const char* const productIdOrAlias);
    bool isPurchaseReady() const;
    void restorePurchases() const;

    void startService();
    void stopService();
    bool isStarted() const;

    Backend& getBackend();

private:
    bool started;
    Backend backend;
    mutable ProductList products;
    std::map<std::string, std::string> productIdAliases;
};

} // namespace payment
} // namespace avalon

#endif /* AVALON_PAYMENT_MANAGER_H */
