#include "CF/Number.hpp"

#include <bit>
#include <limits>
#include <type_traits>

namespace CF
{
// MARK: - Internal Helper Functions -

// MARK: - Constructors -
    // Default to smallest 8-bit type
    Number::Number() :
    _allocator(kCFAllocatorDefault)
    {
        uint8_t zero = 0;
        _cfObject = CFNumberCreate(_allocator, kCFNumberSInt8Type, &zero);
    }
    template <typename T>
        requires std::is_integral_v<T> || std::is_floating_point_v<T>
    Number::Number(const T value, CFAllocatorRef allocator) noexcept :
    _allocator(allocator)
    {
        CFNumberRef number = nullptr;
        static_assert(std::numeric_limits<T>::digits != 0, "Passed in value has no valid digits!");
        switch (std::numeric_limits<T>::digits)
        {
            case std::numeric_limits<int16_t>::digits:
                number = CFNumberCreate(_allocator, kCFNumberSInt16Type, &value);
                break;
            case std::numeric_limits<int32_t>::digits:
                number = CFNumberCreate(_allocator, kCFNumberSInt32Type, &value);
                break;
            case std::numeric_limits<int64_t>::digits:
                number = CFNumberCreate(_allocator, kCFNumberSInt64Type, &value);
                break;
            case std::numeric_limits<float>::digits:
                number = CFNumberCreate(_allocator, kCFNumberFloat32Type, &value);
                break;
            case std::numeric_limits<double>::digits:
                number = CFNumberCreate(_allocator, kCFNumberFloat64Type, &value);
                break;
            default: // Set to smallest size
                number = CFNumberCreate(_allocator, kCFNumberSInt8Type, &value);
                break;
        }
        _cfObject = number;
    }

// MARK: - Operator Overloads -
    template<typename T>
        requires std::is_integral_v<T> || std::is_floating_point_v<T>
    bool Number::operator==(const T value) const noexcept
    {
        bool isEqual;
        CFNumberType numType = CFNumberGetType(static_cast<CFNumberRef>(_cfObject));
        T data = 0;

        if (CFNumberIsFloatType(static_cast<CFNumberRef>(_cfObject)) && std::is_integral_v<T> ||
            !CFNumberIsFloatType(static_cast<CFNumberRef>(_cfObject)) && std::is_floating_point_v<T>)
            return false;
        
        /*
        *   We don't check the bool return value of CFNumberGetValue since it only marks a "lossy" conversion.
        *   The lossyness will be determined in the isEqual check as well.
        */
        CFNumberGetValue(static_cast<CFNumberRef>(_cfObject), numType, &data);
        isEqual = (value == data);

        return isEqual;
    }

    template<typename T>
        requires std::is_integral_v<T> || std::is_floating_point_v<T>
    Number::operator T() const noexcept
    {
        T value;
        CFNumberType numType = CFNumberGetType(static_cast<CFNumberRef>(_cfObject));
        bool wasLossy = CFNumberGetValue(static_cast<CFNumberRef>(_cfObject), numType, &value);
        return value;
    }

// MARK: - Template Function Insantiations -
    // Needed for linker to find symbols
    template Number::Number(const uint8_t value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const int8_t value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const uint16_t value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const int16_t value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const uint32_t value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const int32_t value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const float value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const double value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;

    template bool Number::operator==(const uint8_t value) const noexcept;
    template bool Number::operator==(const int8_t value) const noexcept;
    template bool Number::operator==(const uint16_t value) const noexcept;
    template bool Number::operator==(const int16_t value) const noexcept;
    template bool Number::operator==(const uint32_t value) const noexcept;
    template bool Number::operator==(const int32_t value) const noexcept;
    template bool Number::operator==(const uint64_t value) const noexcept;
    template bool Number::operator==(const int64_t value) const noexcept;
    template bool Number::operator==(const float value) const noexcept;
    template bool Number::operator==(const double value) const noexcept;

    template Number::operator uint8_t() const noexcept;
    template Number::operator int8_t() const noexcept;
    template Number::operator uint16_t() const noexcept;
    template Number::operator int16_t() const noexcept;
    template Number::operator uint32_t() const noexcept;
    template Number::operator int32_t() const noexcept;
    template Number::operator uint64_t() const noexcept;
    template Number::operator int64_t() const noexcept;
    template Number::operator float() const noexcept;
    template Number::operator double() const noexcept;
}