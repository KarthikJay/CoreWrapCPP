#include <CF/Number.hpp>
#include <limits>

#include <iostream>

#define BOOST_TEST_MODULE "CF::Boolean Tests"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(number_constructor)
    BOOST_AUTO_TEST_CASE(default_constructor)
    {
        CF::Number test;
        BOOST_CHECK_EQUAL(test, (uint8_t)0);
        BOOST_CHECK_EQUAL(test, (int16_t)0);
        BOOST_CHECK_NE(test, 0.5f);
    }
    BOOST_AUTO_TEST_CASE(integer_u8max_constructor)
    {
        uint64_t temp = std::numeric_limits<uint8_t>::max();
        CF::Number test(std::numeric_limits<uint8_t>::max());
        BOOST_CHECK_EQUAL(test, std::numeric_limits<uint8_t>::max());
        BOOST_CHECK_EQUAL(test, temp);
    }
    BOOST_AUTO_TEST_CASE(copy_float_constructor)
    {
        CF::Number test = 3.14159265;
        BOOST_CHECK_EQUAL(test, 3.14159265);
    }
    BOOST_AUTO_TEST_CASE(copy_double_constructor)
    {
        float junk = 14.76273;
        CF::Number test = std::numeric_limits<double>::max();
        BOOST_CHECK_EQUAL(test, std::numeric_limits<double>::max());
        BOOST_CHECK_NE(test, junk);
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(number_conversion_operator)
    BOOST_AUTO_TEST_CASE(u8_conversion)
    {
        CF::Number test = (uint8_t) 52;
        uint8_t value = static_cast<uint8_t>(test);
        BOOST_CHECK_EQUAL(value, 52);
    }
    BOOST_AUTO_TEST_CASE(float_conversion)
    {
        CF::Number test = (double) 3.14159265;
        double value = static_cast<double>(test);
        BOOST_CHECK_EQUAL(value, 3.14159265);
    }
BOOST_AUTO_TEST_SUITE_END()