#pragma once

#include "ExportVisibility.h"

#include <CoreFoundation/CoreFoundation.h>
#include <iostream>

namespace CF
{
    class CWPP_API Type
    {
        public:
            CFTypeRef GetCFObject(void) const noexcept { return static_cast<CFTypeRef>(this->_cfObject); }
            friend std::ostream &operator<<(std::ostream &os, const CF::Type &obj) noexcept;
            int64_t GetRetainCount(void) const noexcept { return CFGetRetainCount(_cfObject); }

        protected:
            CFTypeRef _cfObject;
            virtual ~Type() noexcept { CFRelease(_cfObject); }
            CFAllocatorRef GetCFAlloc(void) const noexcept { return CFGetAllocator(_cfObject); }

        private:
            virtual CFTypeID GetTypeID(void) const = 0;
    };
}