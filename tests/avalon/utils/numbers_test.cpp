#include <boost/test/unit_test.hpp>

#include <avalon/utils/numbers.h>

BOOST_AUTO_TEST_SUITE(utils)
BOOST_AUTO_TEST_SUITE(numbers)

BOOST_AUTO_TEST_CASE(hasOppositeSigns)
{
    using avalon::utils::numbers::hasOppositeSigns;

    BOOST_CHECK(hasOppositeSigns(1, -1));
    BOOST_CHECK(hasOppositeSigns(-1, 1));

    BOOST_CHECK(!hasOppositeSigns(1, 1));
    BOOST_CHECK(!hasOppositeSigns(0, 0));
}

BOOST_AUTO_TEST_CASE(isPowerOfTwo)
{
    using avalon::utils::numbers::isPowerOfTwo;

    BOOST_CHECK( isPowerOfTwo(0));
    BOOST_CHECK( isPowerOfTwo(1));
    BOOST_CHECK( isPowerOfTwo(2));
    BOOST_CHECK(!isPowerOfTwo(3));
    BOOST_CHECK( isPowerOfTwo(4));
    BOOST_CHECK(!isPowerOfTwo(5));
    BOOST_CHECK(!isPowerOfTwo(6));
    BOOST_CHECK(!isPowerOfTwo(7));
    BOOST_CHECK( isPowerOfTwo(8));
}

BOOST_AUTO_TEST_CASE(roundUpToNextPowerOfTwo)
{
    using avalon::utils::numbers::roundUpToNextPowerOfTwo;

    BOOST_CHECK_EQUAL(1, roundUpToNextPowerOfTwo(0));
    BOOST_CHECK_EQUAL(1, roundUpToNextPowerOfTwo(1));
    BOOST_CHECK_EQUAL(2, roundUpToNextPowerOfTwo(2));
    BOOST_CHECK_EQUAL(4, roundUpToNextPowerOfTwo(3));
    BOOST_CHECK_EQUAL(4, roundUpToNextPowerOfTwo(4));
    BOOST_CHECK_EQUAL(8, roundUpToNextPowerOfTwo(5));
    BOOST_CHECK_EQUAL(8, roundUpToNextPowerOfTwo(6));
    BOOST_CHECK_EQUAL(8, roundUpToNextPowerOfTwo(7));
    BOOST_CHECK_EQUAL(8, roundUpToNextPowerOfTwo(8));
}

BOOST_AUTO_TEST_CASE(reverseBytes)
{
    using avalon::utils::numbers::reverseBytes;

    BOOST_CHECK_EQUAL(0xFF << 24, reverseBytes(0xFF << 0));
    BOOST_CHECK_EQUAL(0xFF << 16, reverseBytes(0xFF << 8));
    BOOST_CHECK_EQUAL(0xFF << 8,  reverseBytes(0xFF << 16));
}

BOOST_AUTO_TEST_CASE(reverseBytesIgnoresZero)
{
    using avalon::utils::numbers::reverseBytes;

    BOOST_CHECK_EQUAL(0, reverseBytes(0));
}

BOOST_AUTO_TEST_SUITE_END() // numbers
BOOST_AUTO_TEST_SUITE_END() // utils