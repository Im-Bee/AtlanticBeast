#include "AppResources.hpp"

#include "Core.h"
#include <cwchar>

namespace Core
{

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------
AppResources::AppResources()
    : m_wstrExePath(InternalGetExecutablePath())
{ }

#ifdef __linux__

// ---------------------------------------------------------------------------------------------------------------------
::std::wstring AppResources::InternalGetExecutablePath() const
{
    char sPath[AB_LONG_STRING];
    ssize_t uLen = readlink("/proc/self/exe", sPath, sizeof(sPath) - 1);

    if (uLen == -1) {
        AB_LOG(Debug::Error, L"Couldn't get path to executable!!! Falling back to `./`");
        return wstring(L"./");
    }
    

    sPath[uLen] = '\0';

    wchar_t* wc = new wchar_t[uLen + 1];
    mbstowcs (wc, sPath, uLen);

    wstring wstrPath(wc);

    return wstrPath.substr(0, wstrPath.find_last_of('/'));
}

#elif _WIN32

::std::wstring AppResources::InternalGetExecutablePath() const
{
    return wstring(L"./");
}

#endif // !__linux__

} // !Core
