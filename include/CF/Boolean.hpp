#pragma once

#include "TypeInterface.hpp"

namespace CF
{
    class CWPP_API Boolean : public CF::Type
    {
        public:
            Boolean(bool value = false) noexcept;

            // Logical Boolean unary operator
            operator bool() const;

            Boolean &operator=(bool value) noexcept;
            bool operator==(bool value) const noexcept;
            //bool &operator==(const Boolean &value) const noexcept;

            int64_t constexpr GetRetainCount(void) { return 1; };

            friend void swap(Boolean &v1, Boolean &v2) noexcept;

        protected:
            CFTypeID GetTypeID(void) const { return CFBooleanGetTypeID(); }
    };
}