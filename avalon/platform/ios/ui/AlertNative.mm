#include <avalon/ui/Alert.h>

#if TARGET_OS_IPHONE
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface AlertViewProxy : NSObject<UIAlertViewDelegate>
{
    @public avalon::ui::Alert::Callback delegate;
}
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;
@end

@implementation AlertViewProxy
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (!delegate) {
        return;
    }

    NSString* buttonTitle = [alertView buttonTitleAtIndex:buttonIndex];
    delegate(buttonIndex, [buttonTitle UTF8String]);

    [self release];
    [alertView release];
}
@end
#endif

namespace avalon {
namespace ui {

void showAlert(const std::string& title, const std::string& message, const Alert::ButtonList& buttons, Alert::Callback& delegate)
{
#if TARGET_OS_IPHONE
    AlertViewProxy* proxy = [[[[AlertViewProxy alloc] init] autorelease] retain];
    proxy->delegate = delegate;

    NSString* cancelTitle = nil;
    if (buttons.size() > 0) {
        cancelTitle = [NSString stringWithUTF8String:buttons.begin()->second.c_str()];
    }

    UIAlertView* view = [[[[UIAlertView alloc] initWithTitle:[NSString stringWithUTF8String:title.c_str()]
                                                     message:[NSString stringWithUTF8String:message.c_str()]
                                                    delegate:proxy
                                           cancelButtonTitle:cancelTitle
                                           otherButtonTitles:nil] autorelease] retain];


    bool firstSkipped = false;
    for (auto& button : buttons) {
        if (!firstSkipped) {
            firstSkipped = true;
            continue;
        }

        [view addButtonWithTitle:[NSString stringWithUTF8String:button.second.c_str()]];
    }

    [view show];
#endif
}

} // namespace ui
} // namespace avalon