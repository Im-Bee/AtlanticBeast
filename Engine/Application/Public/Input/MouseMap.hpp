#ifndef AB_MOUSE_MAP_H
#define AB_MOUSE_MAP_H

#include "CSystem.hpp"

#include "KeyBind.hpp"

namespace App
{

class BEAST_API MouseMap 
{
public:
    
    MouseMap();
    
    ~MouseMap() = default;

    MouseMap(MouseMap&&) noexcept = default;
    
    MouseMap(const MouseMap&) = default;

public:

    void SetKeyToAction(const InputBind& ib, void* pThis, MouseAction a);

    void UnSetKey(const InputBind& ib);

public:

    void PlayAction(float fX, float fY);

private:

    struct DataForActionReplay
    {
        void* pThis;
        MouseAction action;
    };

private:

    ::std::vector<DataForActionReplay> m_MouseBinds;

};

} // !App

#endif // !AB_MOUSE_MAP_H

