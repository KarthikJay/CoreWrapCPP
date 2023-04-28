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

    template <typename T>
        requires std::is_integral_v<T> || std::is_floating_point_v<T>
    static bool compareToCFNumber(const T value, const CFNumberRef object) noexcept
    {
        bool isEqual;
        CFNumberType numType = CFNumberGetType(object);
        int64_t data = 0;

        if (CFNumberIsFloatType(object) && std::is_integral_v<T> ||
            !CFNumberIsFloatType(object) && std::is_floating_point_v<T>)
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
                // Don't need to check bool return value since this can be a "lossy" conversion from CoreFoundation POV.
                CFNumberGetValue(object, numType, &data);
                isEqual = (value == data);
                break;
            case kCFNumberFloat32Type:
            case kCFNumberFloatType:
            case kCFNumberFloat64Type:
            case kCFNumberDoubleType:
            case kCFNumberCGFloatType:
                // Don't need to check bool return value since this can be a "lossy" conversion from CoreFoundation POV.
                CFNumberGetValue(object, numType, reinterpret_cast<double *>(&data));
                isEqual = (static_cast<double>(value) == std::bit_cast<double>(data));
                break;
        }

        return isEqual;
    }

// MARK: - Constructors -
    Number::Number(int8_t value, CFAllocatorRef allocator) noexcept :
    _allocator(allocator)
    {
        _cfObject = convertToCFNumber(value, _allocator);
    }

// MARK: - Operator Overloads -
    bool Number::operator==(const int8_t value) const noexcept
    {
        return compareToCFNumber(value, static_cast<CFNumberRef>(_cfObject));
    }

    bool Number::operator==(const float value) const noexcept
    {
        return compareToCFNumber(value, static_cast<CFNumberRef>(_cfObject));
    }

    bool Number::operator==(const double value) const noexcept
    {
        return compareToCFNumber(value, static_cast<CFNumberRef>(_cfObject));
    }
}