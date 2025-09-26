#ifndef AB_KEYS_MAP_H
#define AB_KEYS_MAP_H

#include "CSystem.hpp"

#include "Bind.h"
#include "KeyList.hpp"

namespace App
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

    void SetKeyToAction(const AbInputBind& ib, void* pThis, AbAction a);

    void UnSetKey(const AbInputBind& ib);

public:

    void PlayAction(AbKeyId keyCode);

private:

    struct DataForActionReplay
    {
        void* pThis;
        AbAction action;
    };

private:

    ::std::vector<DataForActionReplay> m_vKeys;

};

} // !App

#endif // !AB_KEYS_MAP_H

