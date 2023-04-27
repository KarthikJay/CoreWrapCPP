#pragma once

#include "TypeInterface.hpp"

namespace CF
{
    class CWPP_API Boolean : public CF::Type
    {
        public:
            // Constructors
            Boolean(bool value = false) noexcept;

            // Logical Boolean unary operator
            explicit operator bool();

            Boolean &operator=(bool value) noexcept;
            bool operator==(bool value) const noexcept;

            friend void swap(Boolean &v1, Boolean &v2) noexcept;
            int64_t constexpr GetRetainCount(void) { return 1; };

        protected:
            CFTypeID GetTypeID(void) const { return CFBooleanGetTypeID(); }
    };
}