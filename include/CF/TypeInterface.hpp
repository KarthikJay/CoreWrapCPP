#pragma once

#include "ExportVisibility.h"

#include <CoreFoundation/CoreFoundation.h>
#include <iostream>

namespace CF
{
    class CWPP_API Type
    {
        public:
            // Methods
            int64_t GetRetainCount(void) const noexcept { return CFGetRetainCount(_cfObject); }

            // Operators
            friend std::ostream &operator<<(std::ostream& os, const CF::Type& obj) noexcept;
            friend bool operator==(const Type& lhs, const Type& rhs) noexcept;
            explicit operator const CFTypeRef(void) const noexcept { return static_cast<CFTypeRef>(_cfObject); }

        protected:
            // Members
            CFTypeRef _cfObject;

            // TODO: Implement rule of 5!

            // Constructors
            Type(void) noexcept { _cfObject = nullptr; }
            Type(const Type& object) noexcept { this->_cfObject = object._cfObject; CFRetain(_cfObject); }

            // Destructor
            virtual ~Type() noexcept { CFRelease(_cfObject); }

            // Methods
            CFAllocatorRef GetCFAlloc(void) const noexcept { return CFGetAllocator(_cfObject); }

            // Operators
            // Note: Overriding the return type to void makes this not an 'Assignable' copy assignment.
            //          This has the side effect of not allowing types to be compound assignable (A = B = C), or complex comparison constructions
            //          ex: A = B = C will throw an error. This is acceptable as stylistically I don't agree with it.
            //          ex: for (Type a = lval;;;) will throw an error. This is acceptable since Type is an Abstract Base Class
            void operator=(const Type& copyType) noexcept;

        private:
            virtual CFTypeID GetTypeID(void) const = 0;
    };
}