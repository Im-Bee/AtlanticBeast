#ifndef AB_KEYS_MAP_H
#define AB_KEYS_MAP_H

#include "CSystem.hpp"

#include "KeyBind.hpp"
#include "KeyList.hpp"

namespace Core
{

class BEAST_API KeysMap 
{

    static constexpr size_t AmountOfBindableKeys = AB_KEY_COUNT;

public:
    
    KeysMap();
    
    ~KeysMap() = default;

    KeysMap(KeysMap&&) noexcept = default;
    
    KeysMap(const KeysMap&) = default;

public:

    void SetKeyToAction(const InputBind& ib, void* pThis, Action a);

    void UnSetKey(const InputBind& ib);

public:

    void PlayAction(int8_t keyCode);

private:

    struct DataForActionReplay
    {
        void* pThis;
        Action action;
    };

private:

    ::std::vector<DataForActionReplay> m_Keys;

};

} // !Core

#endif // !AB_KEYS_MAP_H

