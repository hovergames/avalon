#ifndef AVALON_ADS_PROVIDER_REVMOBBRIDGE_H
#define AVALON_ADS_PROVIDER_REVMOBBRIDGE_H

namespace avalon {
namespace ads {
namespace provider {

class RevmobBridge
{
public:
    static void startSession(const char* sessionId);
    static void showFullscreen();
    static void showBanner();
    static void hideBanner();
    static void openAdLink();
    static void showPopup();
    static void enableTestingWithAds();
    static void enableTestingWithoutAds();
    static void disableTesting();
};

} // namespace provider
} // namespace ads
} // namespace avalon

#endif /* AVALON_ADS_PROVIDER_REVMOBBRIDGE_H */
