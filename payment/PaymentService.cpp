//
//  PaymentService.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#include "PaymentService.h"
#include "PaymentListener.h"
#include "PaymentProvider.h"
#include "IniReader.h"
#include "Helpers.h"
#include <string>
#include "ProductNonConsumable.h"
#include "ProductConsumable.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "PaymentProviderIos.h"
#endif

PaymentProvider * PaymentService::paymentProvider = 0;

void PaymentService::initWithIniFile(const char *iniFile)
{
    if (PaymentService::paymentProvider)
    {
        cocos2d::CCLog("Paymentservice already initilized");
        return;
    }

    IniReader config;
    config.loadFile(iniFile);

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        PaymentService::paymentProvider = new PaymentProviderIos();
        Helpers::staticCast<PaymentProviderIos*>(PaymentService::paymentProvider)->setBundleId(config.getValue("general", "iosBundleId"));
    #endif

    IniReader::iniSections *sections = config.getSections();
    for (IniReader::iniSections::iterator section = sections->begin(); section != sections->end(); ++section)
    {
        if (std::strcmp(section->first.c_str(), "general") == 0)
            continue;

        Product *product;

        const char *type = config.getValue(section->first.c_str(), "type");
        if (strcmp(type, "consumable") == 0)
        {
            product = new ProductConsumable();
        } else if (strcmp(type, "non-consumable") == 0) {
            product = new ProductNonConsumable();
        } else {
            assert(false && "Couldn't identify product type from ini file");
        }

        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            const char *iosId = config.getValue(section->first.c_str(), "iosId");
            if (strcmp(iosId, "") == 0) {
                delete product;
                continue;
            }
            product->setNativeId(iosId);
            product->setId(section->first.c_str());
            PaymentService::paymentProvider->addProduct(product);
        #endif
    }
}

void PaymentService::startService()
{
    PaymentService::paymentProvider->init();
}

Product * PaymentService::getProduct(const char *productId)
{
    CCAssert(PaymentService::paymentProvider->getProduct(productId), "product not found");
    return PaymentService::paymentProvider->getProduct(productId);
}

void PaymentService::setPaymentReceiver(PaymentListener *receiver)
{
    PaymentService::paymentProvider->setListener(receiver);
}

void PaymentService::restorePurchases()
{
    PaymentService::paymentProvider->restorePurchases();
}