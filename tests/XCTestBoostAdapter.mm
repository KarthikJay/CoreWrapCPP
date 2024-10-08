// Objective-C Headers
#import <Foundation/Foundation.h>
#import <XCTest/XCTest.h>
#import <objc/runtime.h>

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_parameters.hpp>
#include <boost/test/tree/traverse.hpp>
#include <boost/test/results_collector.hpp>
#include <boost/test/unit_test_log_formatter.hpp>
#include <boost/test/execution_monitor.hpp>

using namespace boost::unit_test;

class XCTestReporter : unit_test_log_formatter
{
    public:
        void set_current_xctestcase(XCTest *test) {_xctest = test;}
        /*
        void log_start(std::ostream &) {};
        void log_build_info(std::ostream &) {};
        void test_unit_start(std::ostream &, test_unit const &) {};
        void test_unit_finish(std::ostream &, test_unit const &, unsigned long) {};
        void test_unit_skipped(std::ostream &, test_unit const &, const_string) {};
        void test_unit_skipped(std::ostream &, test_unit const &) {};
        void test_unit_aborted(std::ostream &, test_unit const &) {};
        */

    private:
        XCTestCase *_xctest;
        bool _failedTest;
        NSString *_sourceFilePath;
        NSUInteger _sourceLineNumber;
        std::stringstream _failureDescription;
}