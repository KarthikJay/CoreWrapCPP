#pragma once

#include "TypeInterface.hpp"

#include <type_traits>
#include <compare>

namespace CF
{
    class CWPP_API Number : public CF::Type
    {
        public:
            // Constructors
            Number() noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            Number(const T value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;

            // Methods
            CFNumberType GetNumberTypeID() const noexcept;

            // Operators
            template<typename T>
                requires std::is_arithmetic_v<T>
            Number operator=(const T value) noexcept;

            template<typename T>
                requires std::is_arithmetic_v<T>
            friend std::strong_ordering operator<=>(const T value, const Number num) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend std::strong_ordering operator<=>(const Number num, const T value) noexcept;
            friend std::strong_ordering operator<=>(const CF::Number& lhs, const CF::Number& rhs) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend bool operator==(const T value, const Number num) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend bool operator==(const Number num, const T value) noexcept;
            friend bool operator==(const CF::Number& lhs, const CF::Number& rhs) noexcept = default;

            template<typename T>
                requires std::is_arithmetic_v<T>
            friend Number operator+(const T value, const Number num) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend Number operator+(const Number num, const T value) noexcept;
            friend Number operator+(const Number lhs, const Number rhs) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend Number operator-(const T value, const Number num) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend Number operator-(const Number num, const T value) noexcept;
            friend Number operator-(const Number lhs, const Number rhs) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend Number operator*(const T value, const Number num) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend Number operator*(const Number num, const T value) noexcept;
            friend Number operator*(const Number lhs, const Number rhs) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend Number operator/(const T value, const Number num) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend Number operator/(const Number num, const T value) noexcept;
            friend Number operator/(const Number lhs, const Number rhs) noexcept;

            template<typename T>
                requires std::is_arithmetic_v<T>
            operator T() const noexcept;

            // Logical Boolean unary operator
            explicit operator bool() const noexcept;
            explicit operator const CFNumberRef() const noexcept { return static_cast<CFNumberRef>(_cfObject); }

        protected:
            CFTypeID GetTypeID() const override { return CFNumberGetTypeID(); }
        private:
    };
}