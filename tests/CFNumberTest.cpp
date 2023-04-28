#include <CF/Number.hpp>

#define BOOST_TEST_MODULE "CF::Boolean Tests"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(number_constructor)
    BOOST_AUTO_TEST_CASE(default_integer_8bit_constructor)
    {
        CF::Number test;
        BOOST_CHECK_EQUAL(test, (int8_t)0);
        BOOST_CHECK_NE(test, 0.5f);
    }
BOOST_AUTO_TEST_SUITE_END()
