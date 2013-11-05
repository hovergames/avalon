#include <avalon/utils/url.h>

#if TARGET_OS_IPHONE
    #include <Foundation/Foundation.h>
    #include <UIKit/UIKit.h>
#else
    #include <string>
#endif

namespace avalon {
namespace utils {
namespace url {

void open(const char* pszUrl)
{
#if TARGET_OS_IPHONE
    NSString* msg = [NSString stringWithCString:pszUrl encoding:NSASCIIStringEncoding];
    NSURL* nsUrl = [NSURL URLWithString:msg];
    [[UIApplication sharedApplication] openURL:nsUrl];
#else
    std::string cmd("open ");
    cmd += pszUrl;
    system(cmd.c_str());
#endif
}

} // namespace url
} // namespace utils
} // namespace avalon