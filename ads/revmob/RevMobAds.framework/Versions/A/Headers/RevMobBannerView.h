#import <UIKit/UIKit.h>
#import "RevMobAdvertisement.h"
#import "RevMobAdsDelegate.h"

/**
 Class responsable for handle the BannerView ad unit.
 
 You must integrate it in you app layout as a regualar UIView, if you don't want to handle
 the layout integration you should use RevMobBanner class.
 */
@interface RevMobBannerView : UIView <RevMobAdvertisement, RevMobAdsDelegate, UIWebViewDelegate>

/**
 The delegate setted on this property is called when ad related events happend, see
 RevMobAdsDelegate for mode details.
 */
@property (nonatomic, assign) id<RevMobAdsDelegate> delegate;

/**
 Use this method to load the ad.
 */
- (void)loadAd;

@end
