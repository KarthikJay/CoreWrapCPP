#include <CF/Number.hpp>
#include <limits>

#include <iostream>
#include <iomanip>

#define BOOST_TEST_MODULE "CF::Boolean Tests"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(number_constructors)
    BOOST_AUTO_TEST_CASE(default_constructor)
    {
        CF::Number test;
        BOOST_CHECK_EQUAL(test, (uint8_t)0);
        BOOST_CHECK_EQUAL(test, (int16_t)0);
        BOOST_CHECK_NE(test, 0.5f);
        BOOST_CHECK_NE(test, (double)0.5);
    }
    BOOST_AUTO_TEST_CASE(u8_max_constructor)
    {
        uint64_t temp = std::numeric_limits<uint8_t>::max();
        CF::Number test(std::numeric_limits<uint8_t>::max());
        uint8_t value = test;
        BOOST_CHECK_EQUAL(test, std::numeric_limits<uint8_t>::max());
        BOOST_CHECK_EQUAL(static_cast<uint8_t>(test), temp);
        BOOST_CHECK_EQUAL(test, value);
        BOOST_CHECK_EQUAL(value, static_cast<uint8_t>(test));
    }
    BOOST_AUTO_TEST_CASE(float_max_constructor)
    {
        CF::Number test((float)std::numeric_limits<float>::max());
        BOOST_CHECK_EQUAL(test, (float)std::numeric_limits<float>::max());
    }
    BOOST_AUTO_TEST_CASE(float_min_constructor)
    {
        CF::Number test((float)std::numeric_limits<float>::min());
        BOOST_CHECK_EQUAL(test, (float)std::numeric_limits<float>::min());
    }
    BOOST_AUTO_TEST_CASE(double_max_constructor)
    {
        CF::Number test((double)std::numeric_limits<double>::max());
        BOOST_CHECK_EQUAL(test, (double)std::numeric_limits<double>::max());
    }
    BOOST_AUTO_TEST_CASE(double_min_constructor)
    {
        CF::Number test((double)std::numeric_limits<double>::min());
        BOOST_CHECK_EQUAL(test, (double)std::numeric_limits<double>::min());
    }
    BOOST_AUTO_TEST_CASE(copy_float_max_constructor)
    {
        CF::Number test = std::numeric_limits<float>::max();
        BOOST_CHECK_EQUAL(test, (float)std::numeric_limits<float>::max());
    }
    BOOST_AUTO_TEST_CASE(copy_double_max_constructor)
    {
        float junk = 14.76273;
        CF::Number test = std::numeric_limits<double>::max();
        BOOST_CHECK_EQUAL(test, (double)std::numeric_limits<double>::max());
        BOOST_CHECK_NE(test, junk);
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(number_conversion_operators)
    BOOST_AUTO_TEST_CASE(u8_conversion)
    {
        CF::Number test = (uint8_t) 52;
        uint8_t value = static_cast<uint8_t>(test);
        BOOST_CHECK_EQUAL(value, 52);
    }
    BOOST_AUTO_TEST_CASE(double_conversion)
    {
        CF::Number test = (double) 3.14159265;
        double value = static_cast<double>(test);
        BOOST_CHECK_EQUAL(value, 3.14159265);
    }
    BOOST_AUTO_TEST_CASE(u8_to_double_conversion)
    {
        CF::Number test = (uint8_t) 128;
        double value = static_cast<uint8_t>(test);
        BOOST_CHECK_EQUAL(value, (double) 128.0000);
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(number_equality_operators)
    BOOST_AUTO_TEST_CASE(u8_equal_true)
    {
        CF::Number test((uint8_t)254);
        bool check = (test == (uint8_t)254);
        BOOST_CHECK_EQUAL(check, true);
    }
BOOST_AUTO_TEST_SUITE_END()