#ifndef __Avalon__Payment__Product__
#define __Avalon__Payment__Product__

#include <string>
#include <boost/utility.hpp>

namespace Avalon {
namespace Payment {

class Manager;

class Product : boost::noncopyable
{
    friend class Manager;

public:
    float price;
    std::string localizedPrice;
    std::string localizedName;
    std::string localizedDescription;

    Product(const char* const productId);
    virtual ~Product();

    const std::string& getProductId() const;

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

} // namespace Payment
} // namespace Avalon

#endif /* __Avalon__Payment__Product__ */