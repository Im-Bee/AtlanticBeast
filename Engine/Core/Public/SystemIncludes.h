#ifndef AB_SYSTEMINCLU_H
#define AB_SYSTEMINCLU_H

#ifdef _WIN32
#   define WIN_LEAN_AND_MEAN
#   include <Windows.h>
#elif __linux__
#   include <X11/Xlib.h>
#endif // !_WIN32
       
#endif // !AB_SYSTEMINCLU_H
