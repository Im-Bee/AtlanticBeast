#ifndef AB_IWINDOW_H
#define AB_IWINDOW_H

namespace Core
{

template<class Derived>
class IWindow
{
public:

    IWindow() = default;
    
    ~IWindow() = default;
    
    template<class U>
    IWindow(const IWindow<U>&) 
    { }

    template<class U>
    IWindow(IWindow<U>&&) 
    { }

public:

    

};

} // !Core

#endif // !AB_IWINDOW_H
