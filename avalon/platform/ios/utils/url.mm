#include <avalon/utils/url.h>

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>

namespace avalon {
namespace utils {
namespace url {

void open(const char* pszUrl)
{
    NSString* msg = [NSString stringWithCString:pszUrl encoding:NSASCIIStringEncoding];
    NSURL* nsUrl = [NSURL URLWithString:msg];
    [[UIApplication sharedApplication] openURL:nsUrl];
}

} // namespace url
} // namespace utils
} // namespace avalon