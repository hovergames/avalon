//
//  Helpers.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 04.04.13.
//
//

#ifndef __Adventures_on_the_farm__Helpers__
#define __Adventures_on_the_farm__Helpers__

#include <cassert>

namespace Helpers
{
    template <bool> struct staticAssert;
    template<> struct staticAssert<true> {};

    template<typename T> inline void safeDelete(T*& a) {
        delete a;
        a = NULL;
    }

    template<typename T> inline void safeDeleteArray(T*& a) {
        delete [] a;
        a = NULL;
    }

    template<typename T, typename F> inline T staticCast(F *from)
    {
        assert(dynamic_cast<T>(from) == static_cast<T>(from) && "staticCast failed!");
        return static_cast<T>(from);
    }

    inline unsigned int reverseBytes(unsigned int value)
    {
        return
        (value & 0xFF) << 24 |
        (value >> 8 & 0xFF) << 16 |
        (value >> 16 & 0xFF) << 8 |
        (value >> 24 & 0xFF);
    }

    inline int swapValues(int var, const int val1, const int val2)
    {
        assert(var == val1 || var == val2);
        return val1 + val2 - var;
    }

    inline void swapVariables(int &var1, int &var2)
    {
        var1 ^= var2 ^= var1 ^= var2;
    }

    inline bool hasOppositeSigns(const int x, const int y)
    {
        return ((x ^ y) < 0);
    }

    inline bool powerOfTwo(const int num) {
        return !(num & (num-1));
    }

    unsigned int roundUpToNextPowerOfTwo(unsigned int x);
    unsigned int reverseBytes(unsigned int value);

    struct DeleteObject
    {
        template <typename T>
        void operator()(const T* ptr) const
        {
            Helpers::safeDelete(ptr);
        }
    };
}

#endif /* defined(__Adventures_on_the_farm__Helpers__) */
