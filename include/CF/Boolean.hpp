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

            /* Manually override the garbage value gotten back from CFGetRetainCount().
               If this object exists it technically has a retain count of 1.
            */
            int64_t constexpr GetRetainCount(void) { return 1; };

        protected:
            CFTypeID GetTypeID(void) const { return CFBooleanGetTypeID(); }
    };
}