#ifndef AVALON_UTILS_NUMBERS_H
#define AVALON_UTILS_NUMBERS_H

namespace avalon {
namespace utils {
namespace numbers {

inline bool hasOppositeSigns(const int x, const int y)
{
    return ((x ^ y) < 0);
}

inline bool isPowerOfTwo(const unsigned int num)
{
    // see http://en.wikipedia.org/wiki/Power_of_2#Fast_algorithm_to_check_if_a_positive_number_is_a_power_of_two
    return !(num & (num - 1));
}

unsigned int roundUpToNextPowerOfTwo(unsigned int x)
{
    x += (x == 0);
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
}

inline unsigned int reverseBytes(unsigned int value)
{
    return
    (value       & 0xFF) << 24 |
    (value >>  8 & 0xFF) << 16 |
    (value >> 16 & 0xFF) <<  8 |
    (value >> 24 & 0xFF);
}

} // namespace numbers
} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_NUMBERS_H */
