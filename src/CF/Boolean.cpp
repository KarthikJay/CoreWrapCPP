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
    Boolean::operator bool() const noexcept
    {
        return static_cast<bool>(CFBooleanGetValue(static_cast<CFBooleanRef>(_cfObject)));
    }

    Boolean &Boolean::operator=(bool value) noexcept
    {
        Type::operator=(convertToCFBool(value));

        return *this;
    }

    bool Boolean::operator==(const Boolean& cfValue) const noexcept
    {
        bool current = CFBooleanGetValue(static_cast<CFBooleanRef>(_cfObject));
        bool test = CFBooleanGetValue(static_cast<CFBooleanRef>(cfValue._cfObject));

        return (current == test);
    }

    bool operator==(const Boolean& cfValue, bool value) noexcept
    {
        bool b = static_cast<bool>(CFBooleanGetValue(static_cast<CFBooleanRef>(cfValue._cfObject)));
        
        return (b == value);
    }

    bool operator==(bool value, const Boolean& cfValue) noexcept
    {
        bool b = static_cast<bool>(CFBooleanGetValue(static_cast<CFBooleanRef>(cfValue._cfObject)));
        
        return (value == b);
    }

    std::strong_ordering operator<=>(const Boolean& cfValue, const bool value) noexcept
    {
        bool b = static_cast<bool>(CFBooleanGetValue(static_cast<CFBooleanRef>(cfValue._cfObject)));

        return b <=> value;
    }

    std::strong_ordering operator<=>(const bool value, const Boolean& cfValue) noexcept
    {
        bool b = static_cast<bool>(CFBooleanGetValue(static_cast<CFBooleanRef>(cfValue._cfObject)));

        return value <=> b;
    }

}
