#include "CF/Number.hpp"

#include <bit>
#include <limits>
#include <variant>
#include <functional>

namespace CF
{
// MARK: - Internal Helper Functions -

    /*
        Note: This function assumes unsigned values to be the same size as signed values.
              It's up to the caller to make sure the value is casted properly for interpretation of the value.
    */
    template <typename T>
            requires std::is_arithmetic_v<T>
    static constexpr CFNumberType ConvertToCFNumberType() noexcept
    {
        CFNumberType finalType;

        switch (std::numeric_limits<T>::digits)
        {
            case std::numeric_limits<int8_t>::digits:
            case std::numeric_limits<uint8_t>::digits:
                finalType = kCFNumberSInt8Type;
                break;
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
        }

        return finalType;
    }

    static constexpr auto ConvertToCPPType(CFNumberType numType) noexcept
    {
        std::variant<int8_t, int16_t, int32_t, int64_t, float, double> cppType;
        assert(numType < kCFNumberMaxType);

        // Note: This switch assumes a conforming 64-bit architecture.
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

        // If architecure is 32-bit, long and int are the same size.
        if constexpr (sizeof(long) == sizeof(int))
            cppType = (numType == kCFNumberLongType) ? cppType : static_cast<int32_t>(0);

        return cppType;
    }

    static Number DoubleVariantOperationHelper(const Number lhs, const Number rhs, auto someOperator)
    {
        auto leftCPPType = ConvertToCPPType(lhs.GetNumberTypeID());
        auto rightCPPType = ConvertToCPPType(rhs.GetNumberTypeID());

        return std::visit([&lhs, &rhs, &someOperator](auto leftData, auto rightData) -> Number
                            {
                                CFNumberGetValue(static_cast<CFNumberRef>(lhs), lhs.GetNumberTypeID(), &leftData);
                                CFNumberGetValue(static_cast<CFNumberRef>(rhs), rhs.GetNumberTypeID(), &rightData);
                                return Number(someOperator(leftData, rightData));
                            }, leftCPPType, rightCPPType);
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    static Number SingleVariantOperationHelper(const Number num, const T value, auto someOperator)
    {
        auto cppType = ConvertToCPPType(num.GetNumberTypeID());

        return std::visit([&num, value, &someOperator](auto data) -> Number
                            {
                                CFNumberGetValue(static_cast<CFNumberRef>(num), num.GetNumberTypeID(), &data);
                                return Number(someOperator(data, value));
                            }, cppType);
    }

// MARK: - Constructors -
    Number::Number() noexcept
    {
        int zero = 0;

        _cfObject = CFNumberCreate(kCFAllocatorDefault, ConvertToCFNumberType<int>(), &zero);
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    Number::Number(const T value, CFAllocatorRef allocator) noexcept
    {
        static_assert(std::numeric_limits<T>::digits != 0, "Passed in value has no valid digits!");
        CFNumberType numType = ConvertToCFNumberType<T>();
        _cfObject = CFNumberCreate(allocator, numType, &value);
    }

// MARK: - Methods -
    CFNumberType Number::GetNumberTypeID() const noexcept
    {
        return CFNumberGetType(static_cast<CFNumberRef>(this->_cfObject));
    }

// MARK: - Operator Overloads -
    template<typename T>
        requires std::is_arithmetic_v<T>
    std::strong_ordering operator<=>(const T value, const Number num) noexcept
    {
        CFComparisonResult result;
        CF::Number temp(value);
        result = CFNumberCompare(static_cast<CFNumberRef>(num._cfObject), static_cast<CFNumberRef>(temp._cfObject), NULL);

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

    template<typename T>
        requires std::is_arithmetic_v<T>
    std::strong_ordering operator<=>(const Number num, const T value) noexcept
    {
        return (value <=> num);
    }

    std::strong_ordering operator<=>(const CF::Number& lhs, const CF::Number& rhs) noexcept
    {
        CFComparisonResult result = CFNumberCompare(static_cast<CFNumberRef>(lhs._cfObject), static_cast<CFNumberRef>(rhs._cfObject), NULL);

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

    template<typename T>
        requires std::is_arithmetic_v<T>
    bool operator==(const T value, const Number num) noexcept
    {
        CFComparisonResult result;
        CF::Number temp(value);
        result = CFNumberCompare(static_cast<CFNumberRef>(num._cfObject), static_cast<CFNumberRef>(temp._cfObject), NULL);

        switch (result)
        {
            case kCFCompareEqualTo:
                return true;
            default:
                return false;
        }
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    bool operator==(const Number num, const T value) noexcept
    {
        return (value == num);
    }

// Needed to get rid of false positive warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wfloat-conversion"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
    template<typename T>
        requires std::is_arithmetic_v<T>
    Number::operator T() const noexcept
    {
        T value;
        auto cppType = ConvertToCPPType(this->GetNumberTypeID());
        const CFNumberRef cfObject = static_cast<CFNumberRef>(*this);
        std::visit([&cfObject, &value](auto data)
                    {
                        CFNumberGetValue(cfObject, CFNumberGetType(cfObject), &data);
                        value = data;
                    }, cppType);

        return value;
    }

    Number::operator bool() const noexcept
    {
        auto cppType = ConvertToCPPType(this->GetNumberTypeID());
        const CFNumberRef cfObject = static_cast<CFNumberRef>(*this);
        return std::visit([&cfObject](auto data) -> bool
                    {
                        bool isValid;
                        CFNumberGetValue(cfObject, CFNumberGetType(cfObject), &data);
                        isValid = (data != 0) ? true : false;
                        return isValid;
                    }, cppType);
    }
#pragma clang diagnostic pop

    template<typename T>
        requires std::is_arithmetic_v<T>
    Number operator+(const T value, const Number num) noexcept
    {
        return SingleVariantOperationHelper(num, value, std::plus<void>{});
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    Number operator+(const Number num, const T value) noexcept
    {
        return SingleVariantOperationHelper(num, value, std::plus<void>{});
    }

    Number operator+(const Number lhs, const Number rhs) noexcept
    {
       return DoubleVariantOperationHelper(lhs, rhs, std::plus<void>{});
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    Number operator-(const T value, const Number num) noexcept
    {
        return SingleVariantOperationHelper(num, value, std::minus<void>{});
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    Number operator-(const Number num, const T value) noexcept
    {
        return SingleVariantOperationHelper(num, value, std::minus<void>{});
    }

    Number operator-(const Number lhs, const Number rhs) noexcept
    {
        return DoubleVariantOperationHelper(lhs, rhs, std::minus<void>{});
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    Number operator*(const T value, const Number num) noexcept
    {
        return SingleVariantOperationHelper(num, value, std::multiplies<void>{});
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    Number operator*(const Number num, const T value) noexcept
    {
        return SingleVariantOperationHelper(num, value, std::multiplies<void>{});
    }

    Number operator*(const Number lhs, const Number rhs) noexcept
    {
        return DoubleVariantOperationHelper(lhs, rhs, std::multiplies<void>{});
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    Number operator/(const T value, const Number num) noexcept
    {
        return SingleVariantOperationHelper(num, value, std::divides<void>{});
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    Number operator/(const Number num, const T value) noexcept
    {
        return SingleVariantOperationHelper(num, value, std::divides<void>{});
    }

    Number operator/(const Number lhs, const Number rhs) noexcept
    {
        return DoubleVariantOperationHelper(lhs, rhs, std::divides<void>{});
    }

    template<typename T>
        requires std::is_arithmetic_v<T>
    Number Number::operator=(const T value) noexcept
    {
        static_assert(std::numeric_limits<T>::digits != 0, "Passed in value has no valid digits!");
        CFNumberType numType = ConvertToCFNumberType<T>();
        CFAllocatorRef allocator = this->GetCFAlloc();
        Type::operator=(CFNumberCreate(allocator, numType, &value));

        return *this;
    }

// MARK: - Template Function Insantiations -

    template Number::Number(const uint8_t value = 0,    CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const int8_t value = 0,     CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const uint16_t value = 0,   CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const int16_t value = 0,    CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const uint32_t value = 0,   CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const int32_t value = 0,    CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const float value = 0,      CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
    template Number::Number(const double value = 0,     CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;

    template bool operator==(const uint8_t value,   const Number num)   noexcept;
    template bool operator==(const int8_t value,    const Number num)   noexcept;
    template bool operator==(const uint16_t value,  const Number num)   noexcept;
    template bool operator==(const int16_t value,   const Number num)   noexcept;
    template bool operator==(const uint32_t value,  const Number num)   noexcept;
    template bool operator==(const int32_t value,   const Number num)   noexcept;
    template bool operator==(const uint64_t value,  const Number num)   noexcept;
    template bool operator==(const int64_t value,   const Number num)   noexcept;
    template bool operator==(const float value,     const Number num)   noexcept;
    template bool operator==(const double value,    const Number num)   noexcept;
    template bool operator==(const Number num,  const uint8_t value)    noexcept;
    template bool operator==(const Number num,  const int8_t value)     noexcept;
    template bool operator==(const Number num,  const uint16_t value)   noexcept;
    template bool operator==(const Number num,  const int16_t value)    noexcept;
    template bool operator==(const Number num,  const uint32_t value)   noexcept;
    template bool operator==(const Number num,  const int32_t value)    noexcept;
    template bool operator==(const Number num,  const uint64_t value)   noexcept;
    template bool operator==(const Number num,  const int64_t value)    noexcept;
    template bool operator==(const Number num,  const float value)      noexcept;
    template bool operator==(const Number num,  const double value)     noexcept;

    template Number operator+(const Number num, const uint8_t value)     noexcept;
    template Number operator+(const Number num, const int8_t value)      noexcept;
    template Number operator+(const Number num, const uint16_t value)    noexcept;
    template Number operator+(const Number num, const int16_t value)     noexcept;
    template Number operator+(const Number num, const uint32_t value)    noexcept;
    template Number operator+(const Number num, const int32_t value)     noexcept;
    template Number operator+(const Number num, const uint64_t value)    noexcept;
    template Number operator+(const Number num, const int64_t value)     noexcept;
    template Number operator+(const Number num, const float value)       noexcept;
    template Number operator+(const Number num, const double value)      noexcept;
    template Number operator+(const uint8_t value,   const Number num)   noexcept;
    template Number operator+(const int8_t value,    const Number num)   noexcept;
    template Number operator+(const uint16_t value,  const Number num)   noexcept;
    template Number operator+(const int16_t value,   const Number num)   noexcept;
    template Number operator+(const uint32_t value,  const Number num)   noexcept;
    template Number operator+(const int32_t value,   const Number num)   noexcept;
    template Number operator+(const uint64_t value,  const Number num)   noexcept;
    template Number operator+(const int64_t value,   const Number num)   noexcept;
    template Number operator+(const float value,     const Number num)   noexcept;
    template Number operator+(const double value,    const Number num)   noexcept;

    template Number operator-(const Number num, const uint8_t value)     noexcept;
    template Number operator-(const Number num, const int8_t value)      noexcept;
    template Number operator-(const Number num, const uint16_t value)    noexcept;
    template Number operator-(const Number num, const int16_t value)     noexcept;
    template Number operator-(const Number num, const uint32_t value)    noexcept;
    template Number operator-(const Number num, const int32_t value)     noexcept;
    template Number operator-(const Number num, const uint64_t value)    noexcept;
    template Number operator-(const Number num, const int64_t value)     noexcept;
    template Number operator-(const Number num, const float value)       noexcept;
    template Number operator-(const Number num, const double value)      noexcept;
    template Number operator-(const uint8_t value,   const Number num)   noexcept;
    template Number operator-(const int8_t value,    const Number num)   noexcept;
    template Number operator-(const uint16_t value,  const Number num)   noexcept;
    template Number operator-(const int16_t value,   const Number num)   noexcept;
    template Number operator-(const uint32_t value,  const Number num)   noexcept;
    template Number operator-(const int32_t value,   const Number num)   noexcept;
    template Number operator-(const uint64_t value,  const Number num)   noexcept;
    template Number operator-(const int64_t value,   const Number num)   noexcept;
    template Number operator-(const float value,     const Number num)   noexcept;
    template Number operator-(const double value,    const Number num)   noexcept;

    template Number operator*(const Number num, const uint8_t value)     noexcept;
    template Number operator*(const Number num, const int8_t value)      noexcept;
    template Number operator*(const Number num, const uint16_t value)    noexcept;
    template Number operator*(const Number num, const int16_t value)     noexcept;
    template Number operator*(const Number num, const uint32_t value)    noexcept;
    template Number operator*(const Number num, const int32_t value)     noexcept;
    template Number operator*(const Number num, const uint64_t value)    noexcept;
    template Number operator*(const Number num, const int64_t value)     noexcept;
    template Number operator*(const Number num, const float value)       noexcept;
    template Number operator*(const Number num, const double value)      noexcept;
    template Number operator*(const uint8_t value,   const Number num)   noexcept;
    template Number operator*(const int8_t value,    const Number num)   noexcept;
    template Number operator*(const uint16_t value,  const Number num)   noexcept;
    template Number operator*(const int16_t value,   const Number num)   noexcept;
    template Number operator*(const uint32_t value,  const Number num)   noexcept;
    template Number operator*(const int32_t value,   const Number num)   noexcept;
    template Number operator*(const uint64_t value,  const Number num)   noexcept;
    template Number operator*(const int64_t value,   const Number num)   noexcept;
    template Number operator*(const float value,     const Number num)   noexcept;
    template Number operator*(const double value,    const Number num)   noexcept;

    template Number operator/(const Number num, const uint8_t value)     noexcept;
    template Number operator/(const Number num, const int8_t value)      noexcept;
    template Number operator/(const Number num, const uint16_t value)    noexcept;
    template Number operator/(const Number num, const int16_t value)     noexcept;
    template Number operator/(const Number num, const uint32_t value)    noexcept;
    template Number operator/(const Number num, const int32_t value)     noexcept;
    template Number operator/(const Number num, const uint64_t value)    noexcept;
    template Number operator/(const Number num, const int64_t value)     noexcept;
    template Number operator/(const Number num, const float value)       noexcept;
    template Number operator/(const Number num, const double value)      noexcept;
    template Number operator/(const uint8_t value,   const Number num)   noexcept;
    template Number operator/(const int8_t value,    const Number num)   noexcept;
    template Number operator/(const uint16_t value,  const Number num)   noexcept;
    template Number operator/(const int16_t value,   const Number num)   noexcept;
    template Number operator/(const uint32_t value,  const Number num)   noexcept;
    template Number operator/(const int32_t value,   const Number num)   noexcept;
    template Number operator/(const uint64_t value,  const Number num)   noexcept;
    template Number operator/(const int64_t value,   const Number num)   noexcept;
    template Number operator/(const float value,     const Number num)   noexcept;
    template Number operator/(const double value,    const Number num)   noexcept;

    template Number Number::operator=(const uint8_t value)  noexcept;
    template Number Number::operator=(const int8_t value)   noexcept;
    template Number Number::operator=(const uint16_t value) noexcept;
    template Number Number::operator=(const int16_t value)  noexcept;
    template Number Number::operator=(const uint32_t value) noexcept;
    template Number Number::operator=(const int32_t value)  noexcept;
    template Number Number::operator=(const uint64_t value) noexcept;
    template Number Number::operator=(const int64_t value)  noexcept;
    template Number Number::operator=(const float value)    noexcept;
    template Number Number::operator=(const double value)   noexcept;

    template Number::operator uint8_t()     const noexcept;
    template Number::operator int8_t()      const noexcept;
    template Number::operator uint16_t()    const noexcept;
    template Number::operator int16_t()     const noexcept;
    template Number::operator uint32_t()    const noexcept;
    template Number::operator int32_t()     const noexcept;
    template Number::operator uint64_t()    const noexcept;
    template Number::operator int64_t()     const noexcept;
    template Number::operator float()       const noexcept;
    template Number::operator double()      const noexcept;
}