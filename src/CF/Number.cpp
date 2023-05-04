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
        static_assert(std::numeric_limits<T>::digits != 0, "Passed in value has no valid digit!");
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
        int64_t data = 0;

        if (CFNumberIsFloatType(static_cast<CFNumberRef>(_cfObject)) && std::is_integral_v<T> ||
            !CFNumberIsFloatType(static_cast<CFNumberRef>(_cfObject)) && std::is_floating_point_v<T>)
            return false;

        switch(numType)
        {
            case kCFNumberSInt8Type:
            case kCFNumberSInt16Type:
            case kCFNumberSInt32Type:
            case kCFNumberSInt64Type:
            case kCFNumberCharType:
            case kCFNumberShortType:
            case kCFNumberIntType:
            case kCFNumberLongType:
            case kCFNumberLongLongType:
            case kCFNumberCFIndexType:
            case kCFNumberNSIntegerType:
                // Don't need to check bool return value since this can be a "lossy" conversion from CoreFoundation.
                CFNumberGetValue(static_cast<CFNumberRef>(_cfObject), numType, &data);
                isEqual = (value == data);
                break;
            case kCFNumberFloat32Type:
            case kCFNumberFloatType:
            case kCFNumberFloat64Type:
            case kCFNumberDoubleType:
            case kCFNumberCGFloatType:
                // Don't need to check bool return value since this can be a "lossy" conversion from CoreFoundation.
                CFNumberGetValue(static_cast<CFNumberRef>(_cfObject), numType, reinterpret_cast<double *>(&data));
                isEqual = (static_cast<double>(value) == std::bit_cast<double>(data));
                break;
        }

        return isEqual;
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
}