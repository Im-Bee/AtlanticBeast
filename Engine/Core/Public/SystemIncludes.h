#ifndef AB_SYSTEMINCLU_H
#define AB_SYSTEMINCLU_H

#include "CSystem.h"

#ifdef __cplusplus
#   include "CSystem.hpp"
#endif // !__cplusplus

#ifdef _WIN32
#   define WIN_LEAN_AND_MEAN
#   include <Windows.h>
#elif __linux__
#   include <X11/Xlib.h>
#   include <X11/Xutil.h>
#endif // !_WIN32
       
#endif // !AB_SYSTEMINCLU_H
