//
//  Helpers.cpp
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 04.04.13.
//
//

#include "Helpers.h"
#include <cassert>

namespace Helpers
{
    unsigned int roundUpToNextPowerOfTwo(unsigned int x) {
        x += (x == 0);
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        return ++x;
    }
}