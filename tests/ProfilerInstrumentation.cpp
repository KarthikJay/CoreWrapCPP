#include <cstdio>
#include <dlfcn.h>
#include <cxxabi.h>

#include <chrono>
#include <stack>
#include <iostream>

// TODO: Figure out a way to get timestamps for functions
// Potential reference implementation here:
// https://github.com/NERSC/timemory/tree/develop/source/tools/timemory-compiler-instrument

using chrono_t = std::chrono::time_point<std::chrono::system_clock>;
using timestack_t = std::stack<chrono_t>;

static timestack_t startTimes;

__attribute__((no_instrument_function))
void addNewStartTime()
{
    startTimes.emplace(std::chrono::system_clock::now());
}

extern "C"
{
    __attribute__((no_instrument_function))
    void __cyg_profile_func_enter(void *this_fn, void *call_site)
    {
        Dl_info finfo;
        dladdr(call_site, &finfo);
        //timepoints.emplace(system_clock::now());
        //std::cout << "Enter: " << std::endl;
        //printf("Enter: %p, function=%s\n", call_site, finfo.dli_sname);
        startTimes.emplace(std::chrono::system_clock::now());
    }

    __attribute__((no_instrument_function))
    void __cyg_profile_func_exit(void *this_fn, void *call_site)
    {
        /*
        auto endTime = system_clock::now();
        system_clock::time_point beginTime = timepoints.top();
        timepoints.pop();
        auto duration = duration_cast<milliseconds>(endTime - beginTime);
        std::cout << caller_name << ": " << duration.count() << "ms" << std::endl;
        */
       //printf("Exit: %p\n", call_site);
    }
}