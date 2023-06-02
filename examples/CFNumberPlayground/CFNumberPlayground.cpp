#include <CoreFoundation/CoreFoundation.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

int main(void)
{
    const int8_t kIntValue = 50; 
    CFNumberRef intValue = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt8Type, &kIntValue);
    double convertedValue = 0.0;
    bool wasLossy = CFNumberGetValue(intValue, kCFNumberFloat64Type, &convertedValue);

    std::cout << std::boolalpha << std::setprecision(2) << "intValue w/ GetValue as Float64 returns: " << (double)convertedValue << " WasLossy: " << (convertedValue == kIntValue) << std::endl;


    return EXIT_SUCCESS;
}