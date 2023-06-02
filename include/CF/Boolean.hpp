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
            operator bool();

            Boolean &operator=(bool value) noexcept;
            bool operator==(const bool value) const noexcept;
            bool operator!=(const bool value) const noexcept;

            /*
                Manually override the garbage value gotten back from CFGetRetainCount().
                If this object exists it technically has a retain count of 1.
            */
            int64_t GetRetainCount(void) const noexcept = delete;

        protected:
            CFTypeID GetTypeID(void) const override { return CFBooleanGetTypeID(); }
    };
}