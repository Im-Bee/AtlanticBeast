#ifndef AB_IWINDOW_H
#define AB_IWINDOW_H

#include "CSystem.hpp"
#include "WindowDesc.h"
#include "SystemIncludes.h"
#include "Core.h"

namespace Core
{

namespace Details
{
#ifdef __linux__

    ::Display* ImplAskForDisplayLinux(const char* pszDisplayName);

    void ImplAskToCloseDisplayLinux(const char* pszDisplayName);

#endif // !__linux__

#ifdef _WIN32

    bool ImplAskForWindowClass(const wchar_t* pszClassName);

	void ImplAskToRegisterWindowClass(WNDCLASSEX& wcex);

	void ImplAskToCloseWindowClass(const wchar_t* pszClassName);

#endif // !_WIN32

}

template<class Derived>
class IWindow
{
public:

    IWindow() = default;
    
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
    { 
        Core::AppStatus::Get().SendOpenedWindowSignal();
        return static_cast<Derived*>(this)->CreateImpl(); 
    }

    void Show()
    { return static_cast<Derived*>(this)->ShowImpl(); }

    void Hide()
    { return static_cast<Derived*>(this)->HideImpl(); }

    void Update()
    { return static_cast<Derived*>(this)->UpdateImpl(); }

    void Destroy()
    {
        Core::AppStatus::Get().SendClosedWindowSignal();
        return static_cast<Derived*>(this)->DestroyImpl(); 
    }

    const WindowDesc& GetWindowDesc() const
    { return static_cast<Derived*>(this)->GetWindowDescImpl(); }

protected:

#ifdef __linux__
       
    ::Display* AskForDisplayLinux(const char* pszDisplayName)
    { return Details::ImplAskForDisplayLinux(pszDisplayName); }

    void AskToCloseDisplayLinux(const char* pszDisplayName)
    { return Details::ImplAskToCloseDisplayLinux(pszDisplayName); }

#endif // !__linux__

#ifdef _WIN32

    bool AskForWindowClass(const wchar_t* pszClassName)
	{ return Details::ImplAskForWindowClass(pszClassName); }

	void AskToRegisterWindowClass(WNDCLASSEX& wcex)
	{ return Details::ImplAskToRegisterWindowClass(wcex); }

	void AskToCloseWindowClass(const wchar_t* pszClassName)
    { return Details::ImplAskToCloseWindowClass(pszClassName); }

#endif // !_WIN32
};

} // !Core

#endif // !AB_IWINDOW_H
