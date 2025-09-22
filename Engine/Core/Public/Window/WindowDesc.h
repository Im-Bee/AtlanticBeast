#ifndef AB_WINDOW_DESC_H
#define AB_WINDOW_DESC_H

#include "SystemIncludes.h"
#include "ExportImport.h"
#include "WindowEvents.h"
#include "Input/InputEvents.h"
    
#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

typedef struct WindowDesc
{
    wchar_t*        Name;
	const wchar_t*  pwszClassName;
    size_t          uNameLen;
    int32_t         Width;
    int32_t         Height;
    int32_t         IsAlive;
    EAbWindowEvents LastEvent;
    uint8_t         bInputSupport;
    AbInputStruct   InputStruct;
    uint32_t        uUinqueIndex;

#ifdef _WIN32
    HWND            Hwnd;
    WNDCLASSEX      Wcex;
	HHOOK		    KeyboardHook;
	HHOOK		    MouseHook;
#elif __linux__
    Display*        DisplayHandle;
    Window          WindowHandle;
    int32_t         Screen;
    int32_t         XiOpcode;
#endif // !_WIN32
} WindowDesc;

BEAST_API WindowDesc CreateWindowDesc(const wchar_t* pwszName, 
                                      size_t uNameLen, 
                                      int32_t width, 
                                      int32_t height);

#ifdef __cplusplus
}
#endif // !__cplusplus

#ifdef __cplusplus
template<size_t uNameLen>
WindowDesc CreateWindowDesc(const wchar_t (&pwszName)[uNameLen], 
                            int32_t width, 
                            int32_t height)
{
    return CreateWindowDesc(pwszName, uNameLen, width, height);
}
#endif // !__cplusplus

#endif // !AB_WINDOW_DESC_H

