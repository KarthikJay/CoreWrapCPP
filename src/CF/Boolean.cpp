#include "CF/Boolean.hpp"

#include <type_traits>

namespace CF
{
// MARK: - Internal Helper Functions -
    template <typename T>
        requires std::is_integral_v<T>
    static CFBooleanRef convertToCFBool(T value)
    {
        return (value != 0) ? kCFBooleanTrue : kCFBooleanFalse;
    }

// MARK: - Constructors -
    Boolean::Boolean(bool value) noexcept
    {
        _cfObject = static_cast<CFBooleanRef>(CFRetain(convertToCFBool(value)));
    }

// MARK: - Boolean Operators -
    Boolean::operator bool()
    {
        return static_cast<bool>(CFBooleanGetValue(static_cast<CFBooleanRef>(_cfObject)));
    }

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
