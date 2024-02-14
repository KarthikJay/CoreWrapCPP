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
        BOOST_CHECK_EQUAL((uint8_t)test, temp);
        BOOST_CHECK_EQUAL(value, test);
        BOOST_CHECK_EQUAL(value, (uint8_t)test);
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
        CF::Number test(std::numeric_limits<double>::max());
        BOOST_CHECK_EQUAL(test, std::numeric_limits<double>::max());
    }
    BOOST_AUTO_TEST_CASE(double_min_constructor)
    {
        CF::Number test(std::numeric_limits<double>::min());
        BOOST_CHECK_EQUAL(test, std::numeric_limits<double>::min());
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
        BOOST_CHECK_EQUAL(test, (uint8_t) 254);
        BOOST_CHECK_EQUAL((uint8_t) 254, test);
        BOOST_CHECK_EQUAL(test, -2);
        BOOST_CHECK_EQUAL(-2, test);
    }
    BOOST_AUTO_TEST_CASE(cfnumber_equal_false)
    {
        CF::Number test_left(1099);
        CF::Number test_right(-500);
        BOOST_CHECK_NE(test_left, test_right);
        BOOST_CHECK_NE(test_right, test_left);
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(number_comparison_operators)
    BOOST_AUTO_TEST_CASE(u8_smaller_true)
    {
        CF::Number test_left(100);
        CF::Number test_right(600);
        BOOST_CHECK(test_left < test_right);
    }
    BOOST_AUTO_TEST_CASE(u8_not_equal)
    {
        CF::Number test_left((uint8_t)100);
        bool test1 = (test_left != (uint8_t)150);
        BOOST_CHECK_EQUAL(test1, true);
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(number_assignment_operators)
    BOOST_AUTO_TEST_CASE(u8_assignment)
    {
        uint8_t val = 5;
        CF::Number test = val;
        BOOST_CHECK_EQUAL(test, val);
    }
    BOOST_AUTO_TEST_CASE(float_max_assignment)
    {
        CF::Number test = std::numeric_limits<float>::max();
        BOOST_CHECK_EQUAL(test, (float)std::numeric_limits<float>::max());
    }
    BOOST_AUTO_TEST_CASE(double_max_assignment)
    {
        float junk = 14.76273;
        CF::Number test = std::numeric_limits<double>::max();
        BOOST_CHECK_EQUAL(test, (double)std::numeric_limits<double>::max());
        BOOST_CHECK_NE(test, junk);
        test = std::numeric_limits<double>::min();
        BOOST_CHECK_NE(junk, test);
    }
    BOOST_AUTO_TEST_CASE(number_assignment)
    {
        CF::Number test1((uint8_t) 254);
        CF::Number test2 = test1;
        BOOST_CHECK_EQUAL(test1, test2);
        BOOST_CHECK_EQUAL(test2, test1);
    }
    BOOST_AUTO_TEST_CASE(three_way_copy_u8_assignment)
    {
        CF::Number testr;
        CF::Number testl;
        CF::Number testnum((uint8_t) 55);

        testr = testl = testnum;
        BOOST_CHECK_EQUAL(testr, testnum);
        BOOST_CHECK_EQUAL(testl, testnum);
        BOOST_CHECK_EQUAL(testnum, (uint8_t) 55);
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(number_arithmetic_operators)
    BOOST_AUTO_TEST_CASE(u8_addition)
    {
        uint8_t temp = 10;
        CF::Number num(static_cast<uint8_t>(5));
        CF::Number test1(temp + num);
        CF::Number test2(num + temp);
        uint8_t test3 = num + static_cast<uint8_t>(25);
        BOOST_CHECK_EQUAL(test1, 15);
        BOOST_CHECK_EQUAL(test1, test2);
        BOOST_CHECK_EQUAL(test3, 30);
    }
    BOOST_AUTO_TEST_CASE(u8_with_float_addition)
    {
        float temp1 = 10.26768;
        uint8_t temp2 = 5;
        CF::Number test1(temp1+temp2);
        BOOST_CHECK_CLOSE((float)test1, 15.26768, 0.0001);
    }
    BOOST_AUTO_TEST_CASE(cfnumber_u8_addition)
    {
        CF::Number lhs(static_cast<uint8_t>(100));
        CF::Number rhs(static_cast<uint8_t>(56));
        lhs = lhs + rhs;
        BOOST_CHECK_EQUAL(lhs, 156);
        CF::Number rhs2(static_cast<uint8_t>(100));
        lhs = lhs + rhs2;
        BOOST_CHECK_EQUAL(lhs, 256);
        lhs = lhs + rhs2;
        BOOST_CHECK_EQUAL(lhs, 356);
    }
    BOOST_AUTO_TEST_CASE(u8_division)
    {
        CF::Number num((uint8_t) 100);
        CF::Number test1 = num / 2;
        CF::Number test2 = 2 / num;

        BOOST_CHECK_EQUAL(test1, (uint8_t)50);
        BOOST_CHECK_EQUAL(test2, (uint8_t)50);
        BOOST_CHECK_EQUAL(test1, test2);
    }
BOOST_AUTO_TEST_SUITE_END()