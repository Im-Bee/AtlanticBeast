#include "Core.h"
#include "IWindow.hpp"

#include <unordered_map>

namespace Core::Details
{

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
bool ImplAskForWindowClass(const wchar_t* pszClassName)
{
    if (pszClassName == NULL) {
        pszClassName = L"";
    }

    if (RegisteredClasses.find(pszClassName) == RegisteredClasses.end()) {
        return false;
    }
    else {
        ++RegisteredClasses[pszClassName];
        return true;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void ImplAskToRegisterWindowClass(WNDCLASSEX& wcex)
{
    if (RegisteredClasses.find(wcex.lpszClassName) != RegisteredClasses.end() && RegisteredClasses[wcex.lpszClassName] != 0) {
        throw AB_EXCEPT("Traying to register a class that already exists!!!");
    }
   
    ++RegisteredClasses[wcex.lpszClassName];

    RegisterClassEx(&wcex);
}

// ---------------------------------------------------------------------------------------------------------------------
void ImplAskToCloseWindowClass(const wchar_t* pszClassName)
{
    if (RegisteredClasses.find(pszClassName) == RegisteredClasses.end() || RegisteredClasses[pszClassName] == 0) {
        throw AB_EXCEPT("Traying to unregister a class that doesn't exists!!!");
    }

    --RegisteredClasses[pszClassName];
    
    if (RegisteredClasses[pszClassName] != 0) {
        return;
    }

    UnregisterClass(pszClassName, GetModuleHandle(NULL));
}

#endif // !__linux__

} // !Core
