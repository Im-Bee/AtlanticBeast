#ifndef AB_WINDOWDESC_H
#define AB_WINDOWDESC_H

#include "CSystem.hpp"
#include "SystemIncludes.h"

namespace Core
{

struct WindowDesc
{
#ifdef _WIN32
	::HWND          Hwnd;
#elif __linux__
    ::Display*      Display;
    ::Window        Window;
    int             Screen;
#endif // !_WIN32
};

} // !Core

#endif // !AB_WINDOWDESC_H

