#include <iostream>
#include <chrono>
#include <stack>
// AppleClang 14 doesn't support std::source_location, need clang 16+
//#include <source_location>

using namespace std::chrono;

// TODO: Figure out a way to get timestamps for functions
// Potential reference implementation here:
// https://github.com/NERSC/timemory/tree/develop/source/tools/timemory-compiler-instrument

static std::stack<system_clock::time_point> timepoints;

extern "C"
{
    __attribute__((no_instrument_function))
    void __cyg_profile_func_enter(void *this_fn, void *call_site)
    {
        timepoints.emplace(system_clock::now());
    }

    __attribute__((no_instrument_function))
    void __cyg_profile_func_exit(void *this_fn, void *call_site, const char *caller_name = __builtin_FUNCTION())
    {
        auto endTime = system_clock::now();
        system_clock::time_point beginTime = timepoints.top();
        timepoints.pop();
        auto duration = duration_cast<milliseconds>(endTime - beginTime);
        std::cout << caller_name << ": " << duration.count() << "ms" << std::endl;
    }
}