#ifndef AB_IWINDOW_H
#define AB_IWINDOW_H

#include "CSystem.hpp"
#include "WindowDesc.h"
#include "SystemIncludes.h"

namespace Core
{

namespace Details
{
#ifdef __linux__

    ::Display* ImplAskForDisplayLinux(const char* pszDisplayName);

    void ImplAskToCloseDisplayLinux(const char* pszDisplayName);

#endif // !__linux__
}

template<class Derived>
class IWindow
{
public:

    IWindow()
    { }
    
    ~IWindow() 
    { 
        this->Destroy();
    }
    
    IWindow(const IWindow<Derived>&) = default;
    
    template<class U>
    IWindow(const IWindow<U>&) = delete;

    IWindow(IWindow<Derived>&&) = default;

    template<class U>
    IWindow(IWindow<U>&&) = delete;

public:

    void Create()
    { return static_cast<Derived*>(this)->CreateImpl(); }

    void Show()
    { return static_cast<Derived*>(this)->ShowImpl(); }

    void Hide()
    { return static_cast<Derived*>(this)->HideImpl(); }

    void Update()
    { return static_cast<Derived*>(this)->UpdateImpl(); }

    void Destroy()
    { return static_cast<Derived*>(this)->DestroyImpl(); }

    const WindowDesc& GetWindowDesc() const
    { return static_cast<Derived*>(this)->GetWindowDescImpl(); }

protected:

#ifdef __linux__
       
    ::Display* AskForDisplayLinux(const char* pszDisplayName)
    { return Details::ImplAskForDisplayLinux(pszDisplayName); }

    void AskToCloseDisplayLinux(const char* pszDisplayName)
    { return Details::ImplAskToCloseDisplayLinux(pszDisplayName); }

#endif // !__linux__
};

} // !Core

#endif // !AB_IWINDOW_H
