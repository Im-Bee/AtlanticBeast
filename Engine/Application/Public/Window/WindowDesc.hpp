#ifndef AB_WINDOW_DESC_H
#define AB_WINDOW_DESC_H

#include "SystemIncludes.h"
#include "ExportImport.h"
#include "WindowEvents.h"
#include "Input/InputEvents.h"

#include "CSystem.hpp"
    

struct WindowDesc
{
    wchar_t*                        Name;
    size_t                          uNameLen;
	const wchar_t*                  pwszClassName;
    int32_t                         Width;
    int32_t                         Height;
    int32_t                         IsAlive;
    int32_t                         LastEvent;
    ::std::queue<AbInputStruct>     InputStruct;
    uint32_t                        uUinqueIndex;

#ifdef _WIN32
    HWND            Hwnd;
    WNDCLASSEX      Wcex;
#elif __linux__
    Display*        DisplayHandle;
    Window          WindowHandle;
    int32_t         Screen;
#endif // !_WIN32
};

BEAST_API WindowDesc CreateWindowDesc(const wchar_t* pwszName, 
                                      size_t uNameLen, 
                                      int32_t width, 
                                      int32_t height);

template<size_t uNameLen>
WindowDesc CreateWindowDesc(const wchar_t (&pwszName)[uNameLen],
                            int32_t width,
                            int32_t height)
{
    return CreateWindowDesc(pwszName, uNameLen, width, height);
}

#endif // !AB_WINDOW_DESC_H

