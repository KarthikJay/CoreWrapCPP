#include "CF/TypeInterface.hpp"

namespace CF
{
    std::ostream &operator<<(std::ostream &os, const CF::Type &obj) noexcept
    {
        CFStringRef description = CFCopyDescription(obj._cfObject);
        CFIndex utf16Length = CFStringGetLength(description);
        // Add extra space for terminating null character
        CFIndex length = CFStringGetMaximumSizeForEncoding(utf16Length, CFStringGetSystemEncoding()) + 1;
        std::string convertedDescription(length, '\0');
        if (CFStringGetCString(description, convertedDescription.data(), length, CFStringGetSystemEncoding()))
        {
            os << convertedDescription;
        }
        else
        {
            os << "[Error]: Cannot copy CoreFoundation object!";
        }

        return os;
    }
};