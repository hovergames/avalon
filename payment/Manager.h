#ifndef __Avalon__Payment__Manager__
#define __Avalon__Payment__Manager__

#include <string>
#include <map>
#include <boost/utility.hpp>
#include "Backend.h"

namespace Avalon {
namespace Payment {

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

    virtual void purchase(Product* const product);
    void purchase(const char* const productIdOrAlias);
    bool isPurchaseReady() const;
    void restorePurchases() const;

    void startService();
    bool isStarted() const;

private:
    bool started;
    Backend backend;
    mutable ProductList products;
    std::map<std::string, std::string> productIdAliases;
};

} // namespace Payment
} // namespace Avalon

#endif /* __Avalon__Payment__Manager__ */