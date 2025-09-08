#ifndef AB_WINDOWDESC_H
#define AB_WINDOWDESC_H

#include "CSystem.hpp"
#include "SystemIncludes.h"

namespace Core
{

struct WindowDesc
{
    friend class IBaseWindow;

    explicit WindowDesc(const wchar_t* pwszName = L"Unknown window name",
                        int_fast16_t width = 1200, 
                        int_fast16_t height = 700)
        : Name(pwszName)
        , Width(width)
        , Height(height)
    { }

    ~WindowDesc() = default;

    WindowDesc(WindowDesc&&) = default;
    WindowDesc(const WindowDesc&) = default;

private:

    ::std::wstring  Name;
    int_fast16_t    Width;
    int_fast16_t    Height;

#ifdef _WIN32
    ::HWND          Hwnd        = NULL;
#elif __linux__
    ::Display*      Display     = NULL;
    ::Window        Window      = 0;
    int             Screen      = 0;
#endif // !_WIN32
};

} // !Core

#endif // !AB_WINDOWDESC_H

