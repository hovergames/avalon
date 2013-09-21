#ifndef AVALON_PAYMENT_MANAGERDELEGATE_H
#define AVALON_PAYMENT_MANAGERDELEGATE_H

namespace avalon {
namespace payment {

class Manager;
class Product;

class ManagerDelegate
{
public:
    virtual void onServiceStarted(Manager* const manager) {};

    virtual void onPurchaseSucceed(Manager* const manager, Product* const product) = 0;
    virtual void onPurchaseFail(Manager* const manager) = 0;

    virtual void onTransactionStart(Manager* const manager) {};
    virtual void onTransactionEnd(Manager* const manager) {};

    virtual void onRestoreSucceed(Manager* const manager) {};
    virtual void onRestoreFail(Manager* const manager) {};
};

} // namespace payment
} // namespace avalon

#endif /* AVALON_PAYMENT_MANAGERDELEGATE_H */
