#ifndef AVALON_PAYMENT_BACKEND_H
#define AVALON_PAYMENT_BACKEND_H

#include <boost/utility.hpp>

namespace avalon {
namespace payment {

class Manager;
class Product;

class Backend : boost::noncopyable
{
public:
    explicit Backend(Manager& manager);
    ~Backend();

    bool isInitialized() const;
    void initialize();
    void shutdown();

    void purchase(Product* const product);
    bool isPurchaseReady() const;

    void restorePurchases() const;

private:
    Manager& manager;
};

} // namespace payment
} // namespace avalon

#endif /* AVALON_PAYMENT_BACKEND_H */
