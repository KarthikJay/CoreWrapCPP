#include "CF/Number.hpp"

#include <bit>
#include <limits>
#include <type_traits>
#include <variant>

namespace CF
{
// MARK: - Internal Helper Functions -

/*
    Note: This function assumes unsigned values to be the same size as signed values.
          It's up to the caller to make sure the value is casted properly for interpretation of the value.
*/
template <typename T>
        requires std::is_arithmetic_v<T>
static constexpr CFNumberType ConvertToCFNumberType()
{
    CFNumberType finalType;

    switch (std::numeric_limits<T>::digits)
    {
        case std::numeric_limits<uint16_t>::digits:
        case std::numeric_limits<int16_t>::digits:
            finalType = kCFNumberSInt16Type;
            break;
        case std::numeric_limits<uint32_t>::digits:
        case std::numeric_limits<int32_t>::digits:
            finalType = kCFNumberSInt32Type;
            break;
        case std::numeric_limits<uint64_t>::digits:
        case std::numeric_limits<int64_t>::digits:
            finalType = kCFNumberSInt64Type;
            break;
        case std::numeric_limits<float>::digits:
            finalType = kCFNumberFloat32Type;
            break;
        case std::numeric_limits<double>::digits:
            finalType = kCFNumberFloat64Type;
            break;
        default: // Set to smallest 8-bit type
            finalType = kCFNumberSInt8Type;
            break;
    }

    return finalType;
}

/*
    Note: This conversion assumes a conforming 64-bit architecture.
*/
static auto ConvertToCPPType(CFNumberType numType)
{
    std::variant<int8_t, int16_t, int32_t, int64_t, float, double> cppType;
    assert(numType < kCFNumberMaxType);
    switch(numType)
    {
        case kCFNumberCharType:
        case kCFNumberSInt8Type:
            cppType = static_cast<int8_t>(0);
            break;
        case kCFNumberShortType:
        case kCFNumberSInt16Type:
            cppType = static_cast<int16_t>(0);
            break;
        case kCFNumberIntType:
        case kCFNumberSInt32Type:
            cppType = static_cast<int32_t>(0);
            break;
        case kCFNumberLongType:
        case kCFNumberLongLongType:
        case kCFNumberSInt64Type:
            cppType = static_cast<int64_t>(0);
            break;
        case kCFNumberFloatType:
        case kCFNumberFloat32Type:
            cppType = static_cast<float>(0);
            break;
        case kCFNumberDoubleType:
        case kCFNumberFloat64Type:
            cppType = static_cast<double>(0);
            break;
    }

    return cppType;
}

// Helper callable struct for std::visit pattern
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// MARK: - Constructors -
    // Default to smallest 8-bit type
    Number::Number(void) noexcept
    {
        int8_t zero = 0;

        _cfObject = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt8Type, &zero);
    }
    template <typename T>
        requires std::is_arithmetic_v<T>
    Number::Number(const T value, CFAllocatorRef allocator) noexcept
    {
        // TODO: Handle case of CF infinity/NaN
        static_assert(std::numeric_limits<T>::digits != 0, "Passed in value has no valid digits!");
        CFNumberType numType = ConvertToCFNumberType<T>();
        _cfObject = CFNumberCreate(allocator, numType, &value);
    }

// MARK: - Operator Overloads -
    bool Number::operator==(const CF::Number value) const noexcept
    {

    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    bool operator==(const T value, const CF::Number num) noexcept
    {
        T data = static_cast<T>(num);
        bool isEqual = (value == data);

        return isEqual;
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    bool operator==(const CF::Number num, const T value) noexcept
    {
        T data = static_cast<T>(num);
        bool isEqual = (data == value);

        return isEqual;
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    bool operator!=(const CF::Number num, const T value) noexcept
    {
        return !(num == value);
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    bool operator!=(const T value, const CF::Number num) noexcept
    {
        return !(value == num);
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    Number::operator T() const noexcept
    {
        T value;
        CFNumberType numType = CFNumberGetType(static_cast<CFNumberRef>(_cfObject));
        auto cppType = ConvertToCPPType(numType);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-conversion"
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
        std::visit(overloaded{
            [&](int8_t data)  {CFNumberGetValue(static_cast<CFNumberRef>(_cfObject), numType, &data); value = data;},
            [&](int16_t data) {CFNumberGetValue(static_cast<CFNumberRef>(_cfObject), numType, &data); value = data;},
            [&](int32_t data) {CFNumberGetValue(static_cast<CFNumberRef>(_cfObject), numType, &data); value = data;},
            [&](int64_t data) {CFNumberGetValue(static_cast<CFNumberRef>(_cfObject), numType, &data); value = data;},
            [&](float data)   {CFNumberGetValue(static_cast<CFNumberRef>(_cfObject), numType, &data); value = data;},
            [&](double data)  {CFNumberGetValue(static_cast<CFNumberRef>(_cfObject), numType, &data); value = data;}
        }, cppType);
#pragma clang diagnostic pop

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

    template bool operator==(const uint8_t value, const CF::Number num) noexcept;
    template bool operator==(const int8_t value, const CF::Number num) noexcept;
    template bool operator==(const uint16_t value, const CF::Number num) noexcept;
    template bool operator==(const int16_t value, const CF::Number num) noexcept;
    template bool operator==(const uint32_t value, const CF::Number num) noexcept;
    template bool operator==(const int32_t value, const CF::Number num) noexcept;
    template bool operator==(const uint64_t value, const CF::Number num) noexcept;
    template bool operator==(const int64_t value, const CF::Number num) noexcept;
    template bool operator==(const float value, const CF::Number num) noexcept;
    template bool operator==(const double value, const CF::Number num) noexcept;
    template bool operator==(const CF::Number num, const uint8_t value) noexcept;
    template bool operator==(const CF::Number num, const int8_t value) noexcept;
    template bool operator==(const CF::Number num, const uint16_t value) noexcept;
    template bool operator==(const CF::Number num, const int16_t value) noexcept;
    template bool operator==(const CF::Number num, const uint32_t value) noexcept;
    template bool operator==(const CF::Number num, const int32_t value) noexcept;
    template bool operator==(const CF::Number num, const uint64_t value) noexcept;
    template bool operator==(const CF::Number num, const int64_t value) noexcept;
    template bool operator==(const CF::Number num, const float value) noexcept;
    template bool operator==(const CF::Number num, const double value) noexcept;

    template bool operator!=(const CF::Number num, const uint8_t value) noexcept;
    template bool operator!=(const CF::Number num, const int8_t value) noexcept;
    template bool operator!=(const CF::Number num, const uint16_t value) noexcept;
    template bool operator!=(const CF::Number num, const int16_t value) noexcept;
    template bool operator!=(const CF::Number num, const uint32_t value) noexcept;
    template bool operator!=(const CF::Number num, const int32_t value) noexcept;
    template bool operator!=(const CF::Number num, const uint64_t value) noexcept;
    template bool operator!=(const CF::Number num, const int64_t value) noexcept;
    template bool operator!=(const CF::Number num, const float value) noexcept;
    template bool operator!=(const CF::Number num, const double value) noexcept;
    template bool operator!=(const uint8_t value, const CF::Number num) noexcept;
    template bool operator!=(const int8_t value, const CF::Number num) noexcept;
    template bool operator!=(const uint16_t value, const CF::Number num) noexcept;
    template bool operator!=(const int16_t value, const CF::Number num) noexcept;
    template bool operator!=(const uint32_t value, const CF::Number num) noexcept;
    template bool operator!=(const int32_t value, const CF::Number num) noexcept;
    template bool operator!=(const uint64_t value, const CF::Number num) noexcept;
    template bool operator!=(const int64_t value, const CF::Number num) noexcept;
    template bool operator!=(const float value, const CF::Number num) noexcept;
    template bool operator!=(const double value, const CF::Number num) noexcept;

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