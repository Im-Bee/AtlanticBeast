#ifndef AB_BASE_WINDOW_DETAILS_H
#define AB_BASE_WINDOW_DETAILS_H

#include "Core.h"
#include "WindowDesc.h"

#ifdef __cplusplus
#ifdef __linux__

BEAST_API Display* ImplAskForDisplayLinux(const char* pszDisplayName);

BEAST_API void ImplAskToCloseDisplayLinux(const char* pszDisplayName);

#endif // !__linux__

#ifdef _WIN32

BEAST_API bool ImplAskForWindowClass(const wchar_t* pwszClassName);

BEAST_API void ImplAskToRegisterWindowClass(WNDCLASSEX& wcex);

BEAST_API void ImplAskToCloseWindowClass(const wchar_t* pwszClassName);

#endif // !_WIN32
#endif // !__cplusplus
    
#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus
       
BEAST_API uint32_t AbCreateImpl(WindowDesc* pWd);

BEAST_API void AbShowImpl(WindowDesc* pWd);

BEAST_API void AbHideImpl(WindowDesc* pWd);

BEAST_API void AbDestroyImpl(WindowDesc* pWd);

BEAST_API void AbUpdateImpl(WindowDesc* pWd);

#ifdef __cplusplus
}
#endif // !__cplusplus
       
#endif // !AB_BASE_WINDOW_DETAILS_H
