#ifndef __Avalon__Payment__Backend__
#define __Avalon__Payment__Backend__

namespace Avalon {
namespace Payment {

class Manager;
class Product;

class Backend
{
public:
    Backend(const Manager& manager);
    ~Backend();

    bool isInitialized() const;
    void initialize();

    void purchase(Product* const product);
    bool canBePurchased() const;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void restorePurchases() const;
#endif

private:
    const Manager& manager;
    bool initialized;
};

} // namespace Payment
} // namespace Avalon

#endif /* __Avalon__Payment__Backend__ */