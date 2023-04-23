#include "CF/Boolean.hpp"

#include <iostream>
#include <type_traits>

namespace CF
{
    template <typename T>
        requires std::is_integral_v<T>
    static CFBooleanRef convertToCFBool(T value)
    {
        return (value != 0) ? kCFBooleanTrue : kCFBooleanFalse;
    }

    void swap(Boolean &v1, Boolean &v2) noexcept
    {
        std::swap(v1._cfObject, v2._cfObject);
    }

    Boolean::Boolean(bool value) noexcept
    {
        _cfObject = static_cast<CFBooleanRef>(CFRetain(convertToCFBool(value)));
    }

// MARK: - Boolean Operators -
    Boolean &Boolean::operator=(bool value) noexcept
    {
        if (_cfObject != nullptr)
            CFRelease(_cfObject);
        _cfObject = static_cast<CFBooleanRef>(CFRetain(convertToCFBool(value)));
        return *this;
    }

    bool Boolean::operator==(bool value) const noexcept
    {
        bool b = static_cast<bool>(CFBooleanGetValue(static_cast<CFBooleanRef>(_cfObject)));
        
        return (value == b);
    }

}
