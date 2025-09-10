#include "Core.h"
#include "IBaseWindow.hpp"

#include <cstring>
#include <unordered_map>

using namespace std;

#ifdef __linux__

// ---------------------------------------------------------------------------------------------------------------------
struct DisplayCount
{
    ::Display* DisplayPtr;
    size_t Count;
};

// Statics // ----------------------------------------------------------------------------------------------------------
static unordered_map<string, DisplayCount> Displays = { };

// ---------------------------------------------------------------------------------------------------------------------
::Display* ImplAskForDisplayLinux(const char* pszDisplayName)
{
    if (pszDisplayName == NULL) {
        pszDisplayName = "";
    }

    if (Displays.find(pszDisplayName) == Displays.end()) {
        Displays[pszDisplayName] = DisplayCount { nullptr, 0 };
    }

    auto& mapped = Displays[pszDisplayName];

    if (mapped.Count == 0) {
        mapped.DisplayPtr = XOpenDisplay(pszDisplayName);
    }

    ++mapped.Count;
    return mapped.DisplayPtr;
}

// ---------------------------------------------------------------------------------------------------------------------
void ImplAskToCloseDisplayLinux(const char* pszDisplayName)
{
    if (pszDisplayName == NULL) {
        pszDisplayName = "";
    }

    if (Displays.find(pszDisplayName) == Displays.end()) {
        throw AB_EXCEPT("Traying to close a conection with display that doesn't exists!!!");
    }

    auto& mapped = Displays[pszDisplayName];

    --mapped.Count;

    if (mapped.Count == 0) {
        XCloseDisplay(mapped.DisplayPtr);
    }
}

#elif _WIN32
// Statics // ----------------------------------------------------------------------------------------------------------
static unordered_map<wstring, size_t> RegisteredClasses = { };

// ---------------------------------------------------------------------------------------------------------------------
bool ImplAskForWindowClass(const wchar_t* pwszClassName)
{
    if (pwszClassName == NULL) {
        pwszClassName = L"";
    }

    if (RegisteredClasses.find(pwszClassName) == RegisteredClasses.end()) {
        return false;
    }
    else {
        ++RegisteredClasses[pwszClassName];
        return true;
    }
}

// Statics // ----------------------------------------------------------------------------------------------------------
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowDesc* pWd = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pWd = (WindowDesc*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWd);

        pWd->Hwnd = hwnd;
    }
    else
    {
        pWd = (WindowDesc*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pWd)
    {
        switch (uMsg)
        {
        case WM_CLOSE:
        {
            pWd->uLastMessage = -1;
            break;
        }
        default:
            break;
        }

        // AB_LOG(Core::Debug::Info, L"pwd->uLastMessage = %u", pWd->uLastMessage);
    }
      
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ---------------------------------------------------------------------------------------------------------------------
void ImplAskToRegisterWindowClass(WNDCLASSEX& wcex)
{
    wstring className = wcex.lpszClassName;

    wcex.lpfnWndProc = WindowProc;

    if (RegisteredClasses.find(className) != RegisteredClasses.end() && 
        RegisteredClasses[className] != 0)
    {
        throw AB_EXCEPT("Traying to register a class that already exists!!!");
    }
   
    ++RegisteredClasses[className];

    RegisterClassEx(&wcex);
}

// ---------------------------------------------------------------------------------------------------------------------
void ImplAskToCloseWindowClass(const wchar_t* pwszClassName)
{
    if (RegisteredClasses.find(wstring(pwszClassName)) == RegisteredClasses.end() || RegisteredClasses[pwszClassName] == 0) {
        throw AB_EXCEPT("Traying to unregister a class that doesn't exists!!!");
    }

    --RegisteredClasses[pwszClassName];
    
    if (RegisteredClasses[pwszClassName] != 0) {
        return;
    }

    UnregisterClass(pwszClassName, GetModuleHandle(NULL));
}

#endif // !__linux__
