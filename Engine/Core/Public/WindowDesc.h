#ifndef AB_WINDOW_DESC_H
#define AB_WINDOW_DESC_H

#include "Core.h"
#include "WindowEvents.h"
#include "InputEvents.h"
    
#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

typedef struct WindowDesc
{
    wchar_t*        Name;
    size_t          uNameLen;
    int32_t         Width;
    int32_t         Height;
    int32_t         IsAlive;
    EAbWindowEvents LastEvent;
    char            bInputSupport;
    AbInputStruct   InputStruct;
    uint32_t        uUinqueIndex;

#ifdef _WIN32
    HWND            Hwnd;
	const wchar_t*  pwszClassName;
    WNDCLASSEX      Wcex;
#elif __linux__
    Display*        DisplayHandle;
    Window          WindowHandle;
    int             Screen;
    int             XiOpcode;
#endif // !_WIN32
} WindowDesc;

BEAST_API WindowDesc CreateWindowDesc(const wchar_t* pwszName, size_t uNameLen, int32_t width, int32_t height);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !AB_WINDOW_DESC_H

