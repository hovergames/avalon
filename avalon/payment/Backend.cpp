#ifndef AVALON_CONFIG_PAYMENT_ENABLED

#include <avalon/payment/Backend.h>

/**
 * @warning This is not a real backend implementation! This is just a dummy
 *          that does nothing but solves compilation errors. Just don't define
 *          AVALON_CONFIG_PAYMENT_ENABLED and you don't have to add the 
 *          platform/$NAME/payment folder with all other dependencies.
 *
 * @warning If you want to use a real payment backend you have to:
 *            1) define AVALON_CONFIG_PAYMENT_ENABLED
 *            2) add platform/$NAME/payemnt to your project
 */

namespace avalon {
namespace payment {

Backend::Backend(Manager& manager): manager(manager) {}
Backend::~Backend() {}

bool Backend::isInitialized() const { return false; }
void Backend::initialize() {}
void Backend::shutdown() {}

void Backend::purchase(Product* const product) {}
bool Backend::isPurchaseReady() const { return false; }

void Backend::restorePurchases() const {}

} // namespace payment
} // namespace avalon

#endif /* AVALON_CONFIG_PAYMENT_ENABLED */
