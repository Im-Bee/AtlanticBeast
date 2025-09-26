#ifndef AB_MOUSE_MAP_H
#define AB_MOUSE_MAP_H

#include "CSystem.hpp"

#include "Bind.h"

namespace App
{

class BEAST_API MouseMap 
{
public:
    
    MouseMap() = default;
    
    ~MouseMap() = default;

    MouseMap(MouseMap&&) noexcept = default;
    
    MouseMap(const MouseMap&) = default;

public:

    void SetKeyToAction(const AbInputBind& ib, void* pThis, AbMouseAction a);

    void UnSetKey(const AbInputBind& ib);

public:

    void PlayAction(float fX, float fY);

private:

    struct DataForActionReplay
    {
        void* pThis;
        AbMouseAction action;
    };

private:

    ::std::vector<DataForActionReplay> m_vMouseBinds;

};

} // !App

#endif // !AB_MOUSE_MAP_H

