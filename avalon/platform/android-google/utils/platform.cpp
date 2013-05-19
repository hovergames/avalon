#include <avalon/utils/platform.h>

namespace avalon {
namespace utils {
namespace platform {

std::string getName()
{
    return std::string("android");
}

std::string getFlavor()
{
    return std::string("google");
}

} // namespace platform
} // namespace utils
} // namespace avalon
