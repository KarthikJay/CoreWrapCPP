#pragma once

#include "TypeInterface.hpp"

namespace CF
{
    class CWPP_API String : public CF::Type
    {
        public:
            // Constructors
            String() noexcept;
            String(const char* value, CFStringEncoding encoding = kCFStringEncodingUTF8);
        private:
    }
}