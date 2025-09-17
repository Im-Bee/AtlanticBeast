#ifndef AB_BASE_WINDOW_DETAILS_H
#define AB_BASE_WINDOW_DETAILS_H

#include "Core.h"
#include "WindowDesc.h"

#ifdef __cplusplus
#ifdef __linux__

BEAST_API Display* AbDetailsAskForDisplayLinux(const char* pszDisplayName);

BEAST_API void AbDetailsAskToCloseDisplayLinux(const char* pszDisplayName);

#endif // !__linux__

#ifdef _WIN32

BEAST_API bool AbDetailsAskForWindowClass(const wchar_t* pwszClassName);

BEAST_API void AbDetailsAskToRegisterWindowClass(WNDCLASSEX& wcex);

BEAST_API void AbDetailsAskToCloseWindowClass(const wchar_t* pwszClassName);

#endif // !_WIN32
#endif // !__cplusplus
    
#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus
       
BEAST_API uint32_t AbDetailsCreateImpl(WindowDesc* pWd);

BEAST_API void AbDetailsShowImpl(WindowDesc* pWd);

BEAST_API void AbDetailsHideImpl(WindowDesc* pWd);

BEAST_API void AbDetailsDestroyImpl(WindowDesc* pWd);

BEAST_API void AbDetailsUpdateImpl(WindowDesc* pWd);

#ifdef __cplusplus
}
#endif // !__cplusplus
       
#endif // !AB_BASE_WINDOW_DETAILS_H
