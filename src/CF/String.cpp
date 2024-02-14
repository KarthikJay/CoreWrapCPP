#include "CF/String.hpp"

#include <cstring>
#include <stdexcept>
#include <format>
#include <sstream>

namespace CF
{
// MARK: - Internal Helper Defines

// MARK: - Internal Helper Functions -

// MARK: - Constructors -
/*
    String::String(const std::string_view aString, CFAllocatorRef alloc, CFStringEncoding encoding)
    {
        if (aString.ends_with('\0'))
            _cfObject = CFStringCreateWithCString(alloc, aString.data(), encoding);
        else
            _cfObject = CFStringCreateWithBytes(alloc, reinterpret_cast<const uint8_t*>(aString.data()), static_cast<CFIndex>(aString.size()), encoding, false);

        if (_cfObject == nullptr)
                throw std::runtime_error(std::string("CoreFoundation could not create valid string with value: ") + aString.data());

        _encoding = encoding;
    }
*/

    String::String(const std::string& cString, CFAllocatorRef alloc, CFStringEncoding encoding) noexcept
    {
        _cfObject = CFStringCreateWithCString(alloc, cString.data(), encoding);
        _encoding = encoding;
    }

// MARK: - Methods -
    std::strong_ordering operator<=>(const String& lhs, const String& rhs) noexcept
    {
        CFComparisonResult result = CFStringCompare(static_cast<CFStringRef>(lhs._cfObject), static_cast<CFStringRef>(rhs._cfObject), String::kDefaultStringCompareOptions);

        switch (result)
        {
            case kCFCompareLessThan:
                return std::strong_ordering::less;
            case kCFCompareGreaterThan:
                return std::strong_ordering::greater;
            case kCFCompareEqualTo:
                return std::strong_ordering::equal;
        }
    }

    std::strong_ordering operator<=>(const std::string& lhs, const String& rhs) noexcept
    {
        String temp(lhs);
        CFComparisonResult result = CFStringCompare(static_cast<CFStringRef>(temp._cfObject), static_cast<CFStringRef>(rhs._cfObject), String::kDefaultStringCompareOptions);

        switch (result)
        {
            case kCFCompareLessThan:
                return std::strong_ordering::less;
            case kCFCompareGreaterThan:
                return std::strong_ordering::greater;
            case kCFCompareEqualTo:
                return std::strong_ordering::equal;
        }
    }

    std::strong_ordering operator<=>(const String& lhs, const std::string& rhs) noexcept
    {
        String temp(rhs);
        CFComparisonResult result = CFStringCompare(static_cast<CFStringRef>(lhs._cfObject), static_cast<CFStringRef>(temp._cfObject), String::kDefaultStringCompareOptions);

        switch (result)
        {
            case kCFCompareLessThan:
                return std::strong_ordering::less;
            case kCFCompareGreaterThan:
                return std::strong_ordering::greater;
            case kCFCompareEqualTo:
                return std::strong_ordering::equal;
        }
    }

    bool operator==(const String& lhs, const String& rhs) noexcept
    {
        if ((lhs <=> rhs) == std::strong_ordering::equal)
            return true;
        else
            return false;
    }

    bool operator==(const std::string& lhs, const String& rhs) noexcept
    {
        if ((lhs <=> rhs) == std::strong_ordering::equal)
            return true;
        else
            return false;
    }

    String String::operator=(const std::string& aString) noexcept
    {
        CFAllocatorRef alloc = this->GetCFAlloc();
        CFStringEncoding current_encoding = this->_encoding;
        _cfObject = CFStringCreateWithCString(alloc, aString.data(), current_encoding);
        return *this;
    }

    std::string String::data(void) const
    {
        std::ostringstream errorMessage;
        CFIndex utf16Length = CFStringGetLength(static_cast<CFStringRef>(_cfObject));
        size_t stringLength = static_cast<size_t>(CFStringGetMaximumSizeForEncoding(utf16Length, _encoding) + 1);
        assert(stringLength >= 0);
        
        std::string cfData(stringLength, '\0');
        assert(cfData.empty() == false);
        if (!CFStringGetCString(static_cast<CFStringRef>(_cfObject), &cfData.front(), static_cast<CFIndex>(stringLength), _encoding))
        {
            //throw std::runtime_error(std::format("[CoreFoundation Error]: Could not stringize object={:#010x}", reinterpret_cast<intptr_t>(_cfObject)));
            errorMessage << "[CoreFoundation Error]: Could not stringize object = " << _cfObject << std::endl;
            throw std::runtime_error(errorMessage.str());
        }
        cfData.resize(strlen(cfData.data()));

        return cfData;
    }

    /*
    String operator+(String& cfString, const std::string_view cString)
    {

    }
    */

    template <typename T>
        requires std::is_arithmetic_v<T>
    String operator+(String& cfString, const T value) noexcept
    {
        //std::string appendedData = std::format("{}{}", cfString.data(), value);
        cfString = cfString.data() + std::to_string(value);

        return cfString;
    }

// MARK: - Template Function Insantiations -

    template String operator+(String& cfString, const uint8_t value)    noexcept;
    template String operator+(String& cfString, const int8_t value)     noexcept;
    template String operator+(String& cfString, const uint16_t value)   noexcept;
    template String operator+(String& cfString, const int16_t value)    noexcept;
    template String operator+(String& cfString, const uint32_t value)   noexcept;
    template String operator+(String& cfString, const int32_t value)    noexcept;
    template String operator+(String& cfString, const uint64_t value)   noexcept;
    template String operator+(String& cfString, const int64_t value)    noexcept;
    template String operator+(String& cfString, const float value)      noexcept;
    template String operator+(String& cfString, const double value)     noexcept;
}