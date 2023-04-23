#include <CF/Boolean.hpp>

#define BOOST_TEST_MODULE "CF::Boolean Tests"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(boolean_constructor)

    BOOST_AUTO_TEST_CASE(default_constructor)
    {
        CF::Boolean test;
        BOOST_CHECK_EQUAL(test, false);
    }

    BOOST_AUTO_TEST_CASE(bool_true)
    {
        CF::Boolean test(true);
        BOOST_CHECK_EQUAL(test, true);
    }

    BOOST_AUTO_TEST_CASE(bool_false)
    {
        CF::Boolean test(false);
        BOOST_CHECK_EQUAL(test, false);
    }

    BOOST_AUTO_TEST_CASE(int_false)
    {
        CF::Boolean test(0);
        sleep(10);
        BOOST_CHECK_EQUAL(test, false);
    }

    BOOST_AUTO_TEST_CASE(int_true)
    {
        CF::Boolean test(1);
        BOOST_CHECK_EQUAL(test, true);
    }

    BOOST_AUTO_TEST_CASE(float_false)
    {
        CF::Boolean test(0.0);
        BOOST_CHECK_EQUAL(test, false);
    }

    BOOST_AUTO_TEST_CASE(float_true)
    {
        CF::Boolean test(1.0);
        BOOST_CHECK_EQUAL(test, true);
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(boolean_assignment)

    BOOST_AUTO_TEST_CASE(reassign_false)
    {
        CF::Boolean test(true);
        test = false;
        BOOST_CHECK_EQUAL(test, false);
    }

BOOST_AUTO_TEST_SUITE_END()