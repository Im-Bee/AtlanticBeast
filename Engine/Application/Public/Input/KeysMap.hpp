#ifndef AB_KEYS_MAP_H
#define AB_KEYS_MAP_H

#include "Bind.h"
#include "KeyList.hpp"
#include "IBindMap.hpp"

namespace App
{

class KeysMap : public IBindMap<KeysMap>
{

    static constexpr size_t AmountOfBindableKeys = AB_KEY_COUNT;

public:
    
    KeysMap();

    ~KeysMap() = default;

    KeysMap(KeysMap&&) noexcept = default;

    KeysMap(const KeysMap&) = default;

public:

    void BindActionImpl(const AbInputBind& ib,
                                  void* pThis,
                                  AbAction a, 
                                  AbMouseAction ma);

    void UnbindActionImpl(const AbInputBind& ib, void* pThis);

public:

    void PlayAction(AbKeyId keyCode);

private:

    struct ActionReplayData
    {
        void* pThis;
        AbAction action;
    };

private:

    ::std::vector<ActionReplayData> m_vKeys;

};

} // !App

#endif // !AB_KEYS_MAP_H

