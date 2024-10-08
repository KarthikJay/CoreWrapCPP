#pragma once

#include "TypeInterface.hpp"
#include <cstddef>

namespace CF
{
    class CWPP_API Dictionary : public Type
    {
        public:
            // Iterator Class
            class CWPP_API Iterator
            {
                using difference_type = std::ptrdiff_t;
                using value_type = CF::Type;
                
                using iterator_category = std::random_access_iterator_tag;
                
            };
            
            // Constructors
            Dictionary();

            // Operators
            friend std::strong_ordering operator<=>(const CF::Dictionary& lhs, const CF::Dictionary& rhs) noexcept;
        protected:
            CFTypeID GetTypeID() const override { return CFDictionaryGetTypeID(); }
        private:


    };
}