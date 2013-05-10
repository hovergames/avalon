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

class Manager : boost::noncopyable
{
public:
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

private:
    bool started;
    Backend backend;
    mutable ProductList products;
    std::map<std::string, std::string> productIdAliases;
};

} // namespace payment
} // namespace avalon

#endif /* AVALON_PAYMENT_MANAGER_H */
