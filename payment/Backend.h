#ifndef __Avalon__Payment__Backend__
#define __Avalon__Payment__Backend__

#include <boost/utility.hpp>

namespace Avalon {
namespace Payment {

class Manager;
class Product;

class Backend : boost::noncopyable
{
public:
    Backend(const Manager& manager);
    ~Backend();

    bool isInitialized() const;
    void initialize();

    void purchase(Product* const product);
    bool isPurchaseReady() const;

    void restorePurchases() const;

private:
    const Manager& manager;
};

} // namespace Payment
} // namespace Avalon

#endif /* __Avalon__Payment__Backend__ */