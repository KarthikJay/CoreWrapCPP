#include "CF/String.hpp"

#include <string_view>
#include <cstring>
#include <stdexcept>
#include <format>
#include <sstream>

namespace CF
{
// MARK: - Internal Helper Defines -

// MARK: - Internal Helper Functions -
static constexpr std::string_view GetEncodingName(CFStringEncoding encoding)
{
    const char* name = nullptr;
    
    switch(encoding)
    {
        case kCFStringEncodingUTF8:
            name = "Unicode (UTF-8)";
            break;
        case kCFStringEncodingUTF16:
            name = "Unicode (UTF-16)";
            break;
        case kCFStringEncodingUTF16BE:
            name = "Unicode (UTF-16BE)";
            break;
        case kCFStringEncodingUTF16LE:
            name = "Unicode (UTF-16LE)";
            break;
        case kCFStringEncodingUTF32:
            name = "Unicode (UTF-32)";
            break;
        case kCFStringEncodingUTF32BE:
            name = "Unicode (UTF-32BE)";
            break;
        case kCFStringEncodingUTF32LE:
            name = "Unicode (UTF-32LE)";
            break;
        case kCFStringEncodingNonLossyASCII:
        case kCFStringEncodingASCII:
            name = "ASCII";
            break;
        case kCFStringEncodingUTF7:
            name = "Unicode (UTF-7)";
            break;
    }

    return name;
}

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

    String::String(const std::string& cString, CFAllocatorRef alloc, CFStringEncoding encoding) :
        Type(CFStringCreateWithCString(alloc, cString.data(), encoding)),
        _encoding(encoding)
    {}

// MARK: - Methods -
    std::strong_ordering operator<=>(const String& lhs, const String& rhs) noexcept
    {
        CFComparisonResult result = CFStringCompare(lhs, rhs, String::kDefaultStringCompareOptions);

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
        Type::operator=(CFStringCreateWithCString(alloc, aString.data(), current_encoding));

        return *this;
    }

    char String::operator[](const size_t idx) const noexcept
    {
        UniChar temp = CFStringGetCharacterAtIndex(static_cast<CFStringRef>(_cfObject), static_cast<long>(idx));

        return static_cast<char>(temp);
    }

    char String::at(const size_t idx) const
    {
        std::ostringstream errorMessage;
        CFStringEncoding stringEncoding = CFStringGetSmallestEncoding(static_cast<CFStringRef>(_cfObject));
        size_t stringLength = static_cast<size_t>(CFStringGetMaximumSizeForEncoding(CFStringGetLength(static_cast<CFStringRef>(_cfObject)), stringEncoding));
        if (GetEncodingName(stringEncoding) != "ASCII")
        {
            errorMessage << "[CoreFoundation Error]: String is encoded in " << GetEncodingName(stringEncoding) << " which is not trivially convertible to char!";
            throw std::runtime_error(errorMessage.str());
        }
        if (idx >= stringLength)
        {
            throw std::out_of_range("Index is greater than CF string length!");
        }
        return (*this)[idx];
    }

    std::string String::data() const
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

    String operator+(const String& cfString, const std::string& addendum)
    {
        String newString = cfString.data() + addendum;

        return newString;
    }

    String operator+(const std::string& aString, const String& cfString)
    {
        String newString = aString + cfString.data();

        return newString;
    }

    String operator+(const String& string1, const String& string2)
    {
        return String(string1.data() + string2.data());
    }

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