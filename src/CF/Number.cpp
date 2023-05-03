#include "CF/Number.hpp"

#include <bit>
#include <limits>
#include <type_traits>

namespace CF
{
// MARK: - Internal Helper Functions -
    template <typename T>
        requires std::is_integral_v<T> || std::is_floating_point_v<T>
    static CFNumberRef convertToCFNumber(const T value, const CFAllocatorRef allocator) noexcept
    {
        CFNumberRef number = nullptr;
        static_assert(std::numeric_limits<T>::digits != 0, "Passed in value has no valid digit!");
        switch (std::numeric_limits<T>::digits)
        {
            case std::numeric_limits<int16_t>::digits:
                number = CFNumberCreate(allocator, kCFNumberSInt16Type, &value);
                break;
            case std::numeric_limits<int32_t>::digits:
                number = CFNumberCreate(allocator, kCFNumberSInt32Type, &value);
                break;
            case std::numeric_limits<int64_t>::digits:
                number = CFNumberCreate(allocator, kCFNumberSInt64Type, &value);
                break;
            case std::numeric_limits<float>::digits:
                number = CFNumberCreate(allocator, kCFNumberFloat32Type, &value);
                break;
            case std::numeric_limits<double>::digits:
                number = CFNumberCreate(allocator, kCFNumberFloat64Type, &value);
                break;
            default: // Set to smallest size
                number = CFNumberCreate(allocator, kCFNumberSInt8Type, &value);
                break;
        }

        return number;
    }

// MARK: - Constructors -
    Number::Number(int8_t value, CFAllocatorRef allocator) noexcept :
    _allocator(allocator)
    {
        _cfObject = convertToCFNumber(value, _allocator);
    }

    Number::Number(int16_t value, CFAllocatorRef allocator) noexcept :
    _allocator(allocator)
    {
        _cfObject = convertToCFNumber(value, _allocator);
    }

    Number::Number(int32_t value, CFAllocatorRef allocator) noexcept :
    _allocator(allocator)
    {
        _cfObject = convertToCFNumber(value, _allocator);
    }

    Number::Number(int64_t value, CFAllocatorRef allocator) noexcept :
    _allocator(allocator)
    {
        _cfObject = convertToCFNumber(value, _allocator);
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
// MARK: - Template Overloads -
    // Needed for linker to find symbols
    template bool Number::operator==<uint8_t>(const uint8_t value) const noexcept;
    template bool Number::operator==<int8_t>(const int8_t value) const noexcept;
    template bool Number::operator==<uint16_t>(const uint16_t value) const noexcept;
    template bool Number::operator==<int16_t>(const int16_t value) const noexcept;
    template bool Number::operator==<uint32_t>(const uint32_t value) const noexcept;
    template bool Number::operator==<int32_t>(const int32_t value) const noexcept;
    template bool Number::operator==<uint64_t>(const uint64_t value) const noexcept;
    template bool Number::operator==<int64_t>(const int64_t value) const noexcept;
    template bool Number::operator==<float>(const float value) const noexcept;
    template bool Number::operator==<double>(const double value) const noexcept;
}