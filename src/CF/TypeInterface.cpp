#include "CF/TypeInterface.hpp"

namespace CF
{
    // TODO: This should probably throw if CF returns a failure to get the string...
    std::ostream &operator<<(std::ostream &os, const Type &obj) noexcept
    {
        CFStringRef description = CFCopyDescription(obj._cfObject);
        CFIndex utf16Length = CFStringGetLength(description);
        // Add extra space for terminating null character - needed since CF Strings are multimodal
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
            os << "[CoreFoundation Error]: Cannot copy CF object!";
        }

        return os;
    }

    bool operator==(const Type& lhs, const Type& rhs) noexcept
    {
        return CFEqual(lhs._cfObject, rhs._cfObject);
    }

    void Type::operator=(const Type& copyType) noexcept
    {
        assert(_cfObject != nullptr);
        assert(this->GetTypeID() == copyType.GetTypeID());
        CFRelease(_cfObject);
        _cfObject = copyType._cfObject;
        CFRetain(_cfObject);
        return;
    }
}