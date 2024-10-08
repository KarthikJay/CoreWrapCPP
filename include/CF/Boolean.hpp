#pragma once

#include "TypeInterface.hpp"

namespace CF
{
    class CWPP_API Boolean : public Type
    {
        public:
            // Constructors
            Boolean(bool value = false) noexcept;

            // Logical Boolean unary operator
            explicit operator bool() const noexcept;

            Boolean &operator=(bool value) noexcept;
            bool operator==(const Boolean& cfValue) const noexcept;
            friend bool operator==(const Boolean& cfValue, const bool value) noexcept;
            friend bool operator==(const bool value, const Boolean& cfValue) noexcept;
            friend std::strong_ordering operator<=>(const Boolean& cfValue, const bool value) noexcept;
            friend std::strong_ordering operator<=>(const bool value, const Boolean& cfValue) noexcept;

            // There is no valid retain count for CF booleans as they are defined constants.
            int64_t GetRetainCount(void) const noexcept = delete;

        protected:
            CFTypeID GetTypeID(void) const override { return CFBooleanGetTypeID(); }
    };
}