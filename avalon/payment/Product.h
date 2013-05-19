#ifndef AVALON_PAYMENT_PRODUCT_H
#define AVALON_PAYMENT_PRODUCT_H

#include <string>
#include <boost/utility.hpp>

namespace avalon {
namespace payment {

class Manager;

class Product : boost::noncopyable
{
    friend class Manager;

public:
    float price;
    std::string localizedPrice;
    std::string localizedName;
    std::string localizedDescription;

    explicit Product(const char* const productId);
    virtual ~Product();

    std::string getProductId() const;

    bool canBePurchased() const;
    void purchase();

    void onHasBeenPurchased();
    bool hasBeenPurchased() const;
    virtual void consume();

protected:
    int purchasedCounter;
    Manager* manager;

private:
    const std::string productId;
};

} // namespace payment
} // namespace avalon

#endif /* AVALON_PAYMENT_PRODUCT_H */
