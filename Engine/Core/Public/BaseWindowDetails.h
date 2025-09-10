#ifndef AB_BASE_WINDOW_DETAILS_H
#define AB_BASE_WINDOW_DETAILS_H

#include "ExportImport.h"
#include "WindowDesc.h"

#ifdef __cplusplus
#ifdef __linux__

BEAST_API Display* ImplAskForDisplayLinux(const char* pszDisplayName);

BEAST_API void ImplAskToCloseDisplayLinux(const char* pszDisplayName);

#endif // !__linux__

#ifdef _WIN32

BEAST_API bool ImplAskForWindowClass(const wchar_t* pszClassName);

BEAST_API void ImplAskToRegisterWindowClass(WNDCLASSEX& wcex);

BEAST_API void ImplAskToCloseWindowClass(const wchar_t* pszClassName);

#endif // !_WIN32
#endif // !__cplusplus
    
#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus
       
BEAST_API uint32_t CreateImpl(WindowDesc* pWd);

BEAST_API void ShowImpl(WindowDesc* pWd);

BEAST_API void HideImpl(WindowDesc* pWd);

BEAST_API void DestroyImpl(WindowDesc* pWd);

BEAST_API void UpdateImpl(WindowDesc* pWd);

#ifdef __cplusplus
}
#endif // !__cplusplus
       
#endif // !AB_BASE_WINDOW_DETAILS_H
