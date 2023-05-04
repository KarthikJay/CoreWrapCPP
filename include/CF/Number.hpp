#pragma once

#include "TypeInterface.hpp"

namespace CF
{
    class CWPP_API Number : public CF::Type
    {
        public:
            // Constructors
            Number();
            template<typename T>
                requires std::is_integral_v<T> || std::is_floating_point_v<T>
            Number(const T value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;


            // Operators
            template<typename T>
                requires std::is_integral_v<T> || std::is_floating_point_v<T>
            bool operator==(const T value) const noexcept;

            template<typename T>
                requires std::is_integral_v<T> || std::is_floating_point_v<T>
            operator T() const noexcept;


            // Logical Boolean unary operator
            //explicit operator bool();

            //Number &operator=(uint32_t value) noexcept;
            //bool operator==(bool value) const noexcept;

        protected:
            CFTypeID GetTypeID(void) const { return CFNumberGetTypeID(); }
        private:
            const CFAllocatorRef _allocator;
    };
}