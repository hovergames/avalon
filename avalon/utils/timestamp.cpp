#include <avalon/utils/timestamp.h>
#include <ctime>

namespace avalon {
namespace utils {

double getUnixTimestamp()
{
    timeval currentTime;
    gettimeofday(&currentTime,  NULL);
    return (currentTime.tv_sec) + (currentTime.tv_usec / 1000000.0);
}

} // namespace utils
} // namespace avalon
