#ifndef __Avalon__Payment__Manager__
#define __Avalon__Payment__Manager__

#include <string>
#include <map>
#include "cocos2d.h"
#include "Backend.h"

namespace Avalon {
namespace Payment {

class ManagerDelegate;
class Product;
class ProductConsumable;
typedef std::map<const std::string, Product* const> ProductList;

class Manager
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

    bool isPurchaseReady() const;
    virtual void purchase(Product* const product);
    void purchase(const char* const productIdOrAlias);

    void startService();
    void restorePurchases() const;

private:
    Backend backend;
    mutable ProductList products;
    std::map<std::string, std::string> productIdAliases;
};

} // namespace Payment
} // namespace Avalon

#endif /* __Avalon__Payment__Manager__ */