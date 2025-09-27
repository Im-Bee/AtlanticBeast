#ifndef AB_MOUSE_MAP_H
#define AB_MOUSE_MAP_H

#include "CSystem.hpp"

#include "Bind.h"
#include "Input/IBindMap.hpp"

namespace App
{

class BEAST_API MouseMap : public IBindMap
{
public:
    
    MouseMap() = default;
    
    ~MouseMap() = default;

    MouseMap(MouseMap&&) noexcept = default;
    
    MouseMap(const MouseMap&) = default;

public:

    virtual void BindAction(const AbInputBind& ib, 
                            void* pThis,
                            AbAction a,
                            AbMouseAction ma) override final;

    virtual void UnbindAction(const AbInputBind& ib, void* pThis) override final;

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

