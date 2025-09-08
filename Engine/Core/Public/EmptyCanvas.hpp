#ifndef AB_EMPTYCANVAS_H
#define AB_EMPTYCANVAS_H

#include "IWindow.hpp"


namespace Core
{

class IBaseWindow : public IWindow<IBaseWindow>
{
    friend IWindow;

#ifdef _WIN32

    template<class T>
    friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

#endif // _WIN32

public:

    template<class U>
    explicit IBaseWindow(U&& windowDesc = WindowDesc())
        : m_WindowDesc(::std::forward<U>(windowDesc))
    { }

    ~IBaseWindow() = default;

    IBaseWindow(const IBaseWindow& other) = default;

    IBaseWindow(IBaseWindow&& other) noexcept
        : m_WindowDesc(::std::move(other.m_WindowDesc))
    { 
        memset(static_cast<void*>(&other.m_WindowDesc), 0, sizeof(WindowDesc));
    }

protected:

    void HandleMessage(int)
    { 
        // Special painting or other stuff can go here
    }

private:

    void CreateImpl();

    void ShowImpl();

    void HideImpl();

    void DestroyImpl();

    void UpdateImpl();

    const WindowDesc& GetWindowDescImpl() const
    { return m_WindowDesc; }

private:

    WindowDesc m_WindowDesc;
    ::std::wstring m_Name;

};

} // !Core

#endif // !AB_EMPTYCANVAS_H
