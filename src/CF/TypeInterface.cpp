#include "CF/TypeInterface.hpp"

namespace CF
{
    std::ostream &operator<<(std::ostream &os, const Type &obj) noexcept
    {
        CFStringRef description = CFCopyDescription(obj._cfObject);
        CFIndex utf16Length = CFStringGetLength(description);
        // Add extra space for terminating null character
        CFIndex length = CFStringGetMaximumSizeForEncoding(utf16Length, CFStringGetSystemEncoding()) + 1;
        // CoreFoundation returned an invalid length for object description!
        assert(length >= 0);
        std::string convertedDescription(static_cast<size_t>(length), '\0');
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

    bool operator==(const Type& lhs, const Type& rhs) noexcept
    {
        return CFEqual(lhs._cfObject, rhs._cfObject);
    }
}