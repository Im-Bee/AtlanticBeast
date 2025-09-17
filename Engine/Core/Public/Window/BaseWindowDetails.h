#ifndef AB_BASE_WINDOW_DETAILS_H
#define AB_BASE_WINDOW_DETAILS_H

#include "ExportImport.h"
#include "SystemIncludes.h"
#include "WindowDesc.h"

#ifdef __cplusplus
#ifdef __linux__

BEAST_API Display* AbAskForDisplayLinux(const char* pszDisplayName);

BEAST_API void AbAskToCloseDisplayLinux(const char* pszDisplayName);

#endif // !__linux__

#ifdef _WIN32

BEAST_API bool AbAskForWindowClass(const wchar_t* pwszClassName);

BEAST_API void AbAskToRegisterWindowClass(WNDCLASSEX& wcex);

BEAST_API void AbAskToCloseWindowClass(const wchar_t* pwszClassName);

#endif // !_WIN32
#endif // !__cplusplus
#endif // !AB_BASE_WINDOW_DETAILS_H
