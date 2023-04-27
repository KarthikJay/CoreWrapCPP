#pragma once

#include "TypeInterface.hpp"

namespace CF
{
    class CWPP_API Number : public CF::Type
    {
        public:
            // Constructors
            Number(uint32_t value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
            //Number(uint32_t value = 0) noexcept;
            //Number(float value = 0.0) noexcept;
            //Number(double value = 0.0) noexcept;

            // Logical Boolean unary operator
            //explicit operator bool();

            //Number &operator=(uint32_t value) noexcept;
            //bool operator==(bool value) const noexcept;

            //friend void swap(Boolean &v1, Boolean &v2) noexcept;

        protected:
            CFTypeID GetTypeID(void) const { return CFNumberGetTypeID(); }
        private:
            const CFAllocatorRef _allocator;
    };
}