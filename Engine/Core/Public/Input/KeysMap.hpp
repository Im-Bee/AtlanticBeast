#ifndef AB_KEYS_MAP_H
#define AB_KEYS_MAP_H

#include "CSystem.hpp"

#include "KeyBind.hpp"
#include "KeyList.hpp"

namespace Core
{

struct PlayableAction
{
    void* pThis;
    Action action;
};


class BEAST_API KeysMap 
{

    static constexpr size_t AmountOfBindableKeys = AB_KEY_COUNT;

public:
    
    KeysMap()
        : m_Keys(AmountOfBindableKeys)
    {
        memset(&m_Keys[0], 0, sizeof(PlayableAction) * m_Keys.size());
    }
    
    ~KeysMap() = default;

    KeysMap(KeysMap&&) noexcept = default;
    
    KeysMap(const KeysMap&) = default;

public:

    void SetKeyToAction(const InputBind& ib, void* pThis, Action a);

public:

    void PlayAction(int8_t keyCode);

private:

    ::std::vector<PlayableAction> m_Keys;

};

} // !Core

#endif // !AB_KEYS_MAP_H

