#ifndef AB_APP_RESOURCES_H
#define AB_APP_RESOURCES_H

#include "Core.h"

namespace App
{

class BEAST_API AppResources
{
    AppResources();

    AppResources(AppResources&&) = delete;
    
    AppResources(const AppResources&) = delete;

public:

    static AppResources& Get()
    {
        static AppResources instance;
        return instance;
    }

    ~AppResources() = default;

public:

    const ::std::wstring& GetExecutablePathW() const
    { return m_wstrExePath; }

    ::std::string GetExecutablePathA() const
    { 
        size_t len = std::wcstombs(nullptr, m_wstrExePath.c_str(), 0);

        if (len != (size_t) - 1) {
            std::string str(len, '\0');
            std::wcstombs(&str[0], m_wstrExePath.c_str(), len);
            return str; 
        }

        return "./";
    }

private:

    ::std::wstring InternalGetExecutablePath() const;

private:

    ::std::wstring m_wstrExePath;

};

} // !App

#endif // !AB_APP_RESOURCES_H
