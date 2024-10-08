#include "CF/Dictionary.hpp"

namespace CF
{
// MARK: - Internal Helper Defines

// MARK: - Internal Helper Functions -

// MARK: - Constructors -
Dictionary::Dictionary()
{
    _cfObject = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    if (_cfObject == nullptr)
        throw std::bad_alloc();
}

// MARK: - Methods -
std::strong_ordering operator<=>(const CF::Dictionary& lhs, const CF::Dictionary& rhs) noexcept
{
    
}

// MARK: - Template Function Insantiations -
}