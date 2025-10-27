#include "App.h"

#include "AppResources.hpp"

namespace App
{

using namespace std;
using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
AppResources::AppResources()
    : m_wstrExePathW(InternalGetExecutablePathW())
    , m_strExePathA(InternalGetExecutablePathA())
{ }

#ifdef __linux__

// ---------------------------------------------------------------------------------------------------------------------
::std::wstring AppResources::InternalGetExecutablePathW() const
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

::std::string AppResources::InternalGetExecutablePathA() const
{
    size_t len = std::wcstombs(nullptr, m_wstrExePathW.c_str(), 0);
    
    if (len != static_cast<size_t>(-1)) {
        std::string str(len, '\0');
        std::wcstombs(&str[0], m_wstrExePathW.c_str(), len);
        return str; 
    }
    
    return "./";
}

} // !App
