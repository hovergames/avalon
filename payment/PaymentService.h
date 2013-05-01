//
//  PaymentService.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 09.04.13.
//
//

#ifndef __Adventures_on_the_farm__PaymentService__
#define __Adventures_on_the_farm__PaymentService__

class Product;
class PaymentListener;
class PaymentProvider;

class PaymentService
{
    PaymentService() {}
    ~PaymentService() {}

    static PaymentProvider *paymentProvider;
    
public:
    static void initWithIniFile(const char *iniFile);
    static void startService();
    static Product* getProduct(const char *productId);
    static void setPaymentReceiver(PaymentListener *receiver);
    static void restorePurchases();
};
#endif /* defined(__Adventures_on_the_farm__PaymentService__) */
