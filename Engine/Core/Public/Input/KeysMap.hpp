#ifndef AB_KEYS_MAP_H
#define AB_KEYS_MAP_H

#include "CSystem.hpp"

#include "KeyList.hpp"

namespace Core
{

class BEAST_API KeysMap 
{

    static constexpr size_t AmountOfBindableKeys = AB_KEY_F12 + 1;

public:
    
    KeysMap()
        : m_Keys(AmountOfBindableKeys)
    {
        ::std::fill(m_Keys.begin(), m_Keys.end(), nullptr);
    }
    
    ~KeysMap() = default;

    KeysMap(KeysMap&&) noexcept = default;
    
    KeysMap(const KeysMap&) = default;

public:



private:

    ::std::vector<int*> m_Keys;   

};

} // !Core

#endif // !AB_KEYS_MAP_H

