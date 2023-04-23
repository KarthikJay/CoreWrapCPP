#include <CoreFoundation/CoreFoundation.h>
#include <iostream>
#include <stdlib.h>

int main(void)
{
    CFBooleanRef current;
    std::cout << "Starting Retain Leaker!" << std::endl;
    for (int i = 0; i <= 100; i++)
    {
        current = static_cast<CFBooleanRef>(CFRetain(kCFBooleanTrue));
        CFRetain(current);
    }
    std::cout << "Finished making retain leaks" << std::endl;
    return EXIT_SUCCESS;
}