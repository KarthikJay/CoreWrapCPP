#include "CF/Number.hpp"

namespace CF
{
// MARK: - Internal Helper Functions -

// MARK: - Constructors -
    Number::Number(uint32_t value, CFAllocatorRef allocator) noexcept :
    _allocator(allocator)
    {
        _cfObject = CFNumberCreate(_allocator, kCFNumberSInt32Type, &value);
    }
}