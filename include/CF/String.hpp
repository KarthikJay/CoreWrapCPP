#pragma once

#include "TypeInterface.hpp"
#include <string_view>

namespace CF
{
    class CWPP_API String : public CF::Type
    {
        // TO-DO: Implement rule of 5
        public:
            // Constructors
            String(const std::string& cString, CFAllocatorRef alloc = kCFAllocatorDefault, CFStringEncoding encoding = String::GetSystemEncoding()) noexcept;

            // Static Definitions
            static constexpr CFStringCompareFlags kDefaultStringCompareOptions = 0;

            // Static Methods
            static CFStringEncoding GetSystemEncoding(void) { return CFStringGetSystemEncoding(); }
            static bool IsEncodingAvailable(CFStringEncoding encoding) { return CFStringIsEncodingAvailable(encoding); }

            // Methods
            friend std::strong_ordering operator<=>(const String& lhs, const String& rhs) noexcept;
            friend std::strong_ordering operator<=>(const std::string& lhs, const String& rhs) noexcept;
            friend std::strong_ordering operator<=>(const String& lhs, const std::string& rhs) noexcept;

            friend bool operator==(const String& lhs, const String& rhs) noexcept;
            friend bool operator==(const std::string& lhs, const String& rhs) noexcept;

            template <typename T>
                requires std::is_arithmetic_v<T>
            friend String operator+(String& cfString, const T value) noexcept;
            template <typename T>
                requires std::is_arithmetic_v<T>
            friend String operator+(const T value, String& cfString) noexcept;
            friend String operator+(String& cfString, const std::string& addendum);

            String operator=(const std::string& aString) noexcept;

            std::string data(void) const;

            explicit operator const CFStringRef(void) const noexcept { return static_cast<CFStringRef>(_cfObject); }
        protected:
            CFTypeID GetTypeID(void) const override { return CFStringGetTypeID(); }
        private:
            CFStringEncoding _encoding;
    };
}