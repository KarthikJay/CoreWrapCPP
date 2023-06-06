#pragma once

#include "TypeInterface.hpp"

#include <type_traits>

namespace CF
{
    class CWPP_API Number : public CF::Type
    {
        public:
            // Constructors
            Number(void) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            Number(const T value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;

            // Operators
            bool operator==(const CF::Number value) const noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend bool operator==(const T value, const CF::Number num) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend bool operator==(const CF::Number num, const T value) noexcept;

            bool operator!=(const CF::Number value) const noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend bool operator!=(const CF::Number num, const T value) noexcept;
            template<typename T>
                requires std::is_arithmetic_v<T>
            friend bool operator!=(const T value, const CF::Number num) noexcept;

            template<typename T>
                requires std::is_arithmetic_v<T>
            CF::Number operator=(const T value) noexcept;


            template<typename T>
                requires std::is_arithmetic_v<T>
            operator T() const noexcept;

            // Logical Boolean unary operator
            explicit operator bool() const noexcept;

            //Number &operator=(uint32_t value) noexcept;
            //bool operator==(bool value) const noexcept;

        protected:
            CFTypeID GetTypeID(void) const { return CFNumberGetTypeID(); }
        private:
    };
}