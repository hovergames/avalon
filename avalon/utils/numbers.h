#ifndef AVALON_UTILS_NUMBERS_H
#define AVALON_UTILS_NUMBERS_H

#include <exception>

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

// A static_cast<> that throws std::bad_cast if the type has been narrowed.
//
// Use boost::numeric_cast<> if you accept some loss (e.g float -> int) but
// want to ensure the range (aka detect under- / overflows). So this cast is to
// static_cast<> what "int i {X}" is to "int i = X".
//
// Found in / Credit goes to:
//     "The C++ Programming Language" by Bjarne Stourstrup
//     Fourth Edition; Page 299; Chapter 11.5 Explicit Type Convertion
template<typename Target, typename Source>
Target narrow_cast(Source value)
{
    auto result = static_cast<Target>(value);
    if (static_cast<Source>(result) != value) {
        throw std::bad_cast();
    }
    return result;
}

} // namespace numbers
} // namespace utils
} // namespace avalon

#endif /* AVALON_UTILS_NUMBERS_H */
