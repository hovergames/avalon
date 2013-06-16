#ifndef AVALON_PAYMENT_BACKEND_H
#define AVALON_PAYMENT_BACKEND_H

#include <boost/utility.hpp>

namespace avalon {
namespace payment {

class Manager;
class Product;

/**
 * The payment backend is responsible for all the native stuff that has to be
 * done.
 *
 * @warning This is _not_ an interface and the use hast to choose the right
 *          implementation from avalon/platform!
 */
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
