#include <CF/String.hpp>

#include <iostream>
#include <iomanip>
using namespace std::literals;

#define BOOST_TEST_MODULE "CF::String Tests"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(string_constructors)
    BOOST_AUTO_TEST_CASE(cstring_constructor)
    {
        CF::String test("Test");
        BOOST_CHECK_EQUAL(test, "Test");
        BOOST_CHECK_EQUAL("Test", test);
    }
    BOOST_AUTO_TEST_CASE(cstring_copy_constructor)
    {
        /*
        CF::String testSV = "StringViewTest"sv;
        BOOST_CHECK_EQUAL(testSV, "StringViewTest");
        */
        CF::String testSL = "StringLiteralTest"s;
        BOOST_CHECK_EQUAL(testSL, "StringLiteralTest");
        BOOST_CHECK_EQUAL("StringLiteralTest", testSL);
    }
BOOST_AUTO_TEST_SUITE_END() 

BOOST_AUTO_TEST_SUITE(string_equality_operators)
    BOOST_AUTO_TEST_CASE(cstring_equality)
    {
        CF::String test("Garbage");
        test = "TestString";
        BOOST_CHECK_EQUAL(test, "TestString");
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(string_relational_operators)
    BOOST_AUTO_TEST_CASE(cstring_lesser)
    {
        CF::String test("A");
        BOOST_CHECK_LT("A", "a");
        BOOST_CHECK_LT(test, "a");
    }
    BOOST_AUTO_TEST_CASE(cstring_greater)
    {
        CF::String test("Z");
        BOOST_CHECK_GT("Z", "A");
        BOOST_CHECK_GT(test, "A");
        BOOST_CHECK_GT(test, "B");
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(string_accessors)
    BOOST_AUTO_TEST_CASE(cstring_data)
    {
        CF::String test("Hello World!");
        BOOST_CHECK_EQUAL(test.data(), "Hello World!");
        BOOST_CHECK_EQUAL("Hello World!", test.data());
        BOOST_CHECK_NE(test.data(), "hello world!");
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(string_appends)
    BOOST_AUTO_TEST_CASE(u8_append)
    {
        CF::String test("Number ");
        test = test + (uint8_t) 5;
        BOOST_CHECK_EQUAL(test, "Number 5");
        BOOST_CHECK_EQUAL("Number 5", test);
    }
BOOST_AUTO_TEST_SUITE_END()