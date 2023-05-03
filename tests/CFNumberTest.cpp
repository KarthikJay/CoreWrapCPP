#include <CF/Number.hpp>
#include <limits>

#include <iostream>

#define BOOST_TEST_MODULE "CF::Boolean Tests"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(number_constructor)
    BOOST_AUTO_TEST_CASE(default_integer_64bit_constructor)
    {
        CF::Number test;
        BOOST_CHECK_EQUAL(test, (uint8_t)0);
        BOOST_CHECK_EQUAL(test, (int16_t)0);
        BOOST_CHECK_NE(test, 0.5f);
    }
    BOOST_AUTO_TEST_CASE(integer_64bit_8bitmax_constructor)
    {
        uint64_t temp = std::numeric_limits<uint8_t>::max();
        CF::Number test(std::numeric_limits<uint8_t>::max());
        BOOST_CHECK_EQUAL(test, std::numeric_limits<uint8_t>::max());
        BOOST_CHECK_EQUAL(test, temp);
    }
BOOST_AUTO_TEST_SUITE_END()
