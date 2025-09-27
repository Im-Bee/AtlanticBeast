#ifndef AB_BIND_MAP_H
#define AB_BIND_MAP_H

namespace App
{

class IBindMap
{
public:

    virtual ~IBindMap() = default;

public:

    virtual void BindAction(const AbInputBind& ib, 
                            void* pThis,
                            AbAction a,
                            AbMouseAction ma) = 0;

    virtual void UnbindAction(const AbInputBind& ib, void* pThis) = 0;

};

} // !Core

#endif // !AB_BIND_MAP_H
