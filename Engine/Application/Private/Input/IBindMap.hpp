#ifndef AB_BIND_MAP_H
#define AB_BIND_MAP_H

#include "Input/Bind.h"

namespace App
{

template<class Map>
class IBindMap
{
public:


    IBindMap() = default;
    ~IBindMap() = default;


public:

    void BindAction(const AbInputBind& ib, 
                    void* pThis,
                    AbAction a,
                    AbMouseAction ma) 
    { static_cast<Map*>(this)->BindActionImpl(ib, pThis, a, ma); }

    void UnbindAction(const AbInputBind& ib, void* pThis)
    { static_cast<Map*>(this)->UnbindActionImpl(ib, pThis); }

};

} // !Core
#endif // !AB_BIND_MAP_H
