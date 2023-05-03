#pragma once

#include "TypeInterface.hpp"

namespace CF
{
    class CWPP_API Number : public CF::Type
    {
        public:
            // Constructors
            explicit Number(int8_t value, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
            explicit Number(int16_t value, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
            explicit Number(int32_t value, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
            explicit Number(int64_t value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;
            //Number(int16_t value = 0, CFAllocatorRef allocator = kCFAllocatorDefault) noexcept;


            // Operators
            template<typename T>
                requires std::is_integral_v<T> || std::is_floating_point_v<T>
            bool operator==(const T value) const noexcept;
            // Forward declare template specializations for linker
            /*
            template<uint8_t> bool operator==(const uint8_t value) const noexcept;
            template<int8_t> bool operator==(const int8_t value) const noexcept;
            template<uint16_t> bool operator==(const uint16_t value) const noexcept;
            template<int16_t> bool operator==(const int16_t value) const noexcept;
            template<uint32_t> bool operator==(const uint32_t value) const noexcept;
            template<int32_t> bool operator==(const int32_t value) const noexcept;
            template<uint64_t> bool operator==(const uint64_t value) const noexcept;
            template<int64_t> bool operator==(const int64_t value) const noexcept;
            template<float> bool operator==(const float value) const noexcept;
            template<double> bool operator==(const double value) const noexcept;
            */



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