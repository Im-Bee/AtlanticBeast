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

#endif // !__linux__

} // !Core
