#ifndef AB_APPSTATUS_H
#define AB_APPSTATUS_H

#include "Core.h"
#include "Window/WindowDesc.hpp"

namespace App
{

enum EAppStatus
{
    Dead,
    Running
};


class BEAST_API AppStatus
{
    template<typename T, typename U>
    friend class IBaseWindow;

    AppStatus();

    static EAppStatus m_AppCurrentStatus;

public:

    static AppStatus& Get();

    ~AppStatus();

public:

    static EAppStatus GetAppCurrentStatus();

    const ::std::list<::std::shared_ptr<WindowDesc>>& GetWindowHandles() const
    { return m_WindowHandles; }

private:

    uint32_t SendCloseWindowSignal(::std::shared_ptr<WindowDesc> pWd);

    uint32_t SendOpenWindowSignal(::std::shared_ptr<WindowDesc> pWd);

    void UpdateStatus();

private:

    uint32_t m_uNumberOfWindows;

    ::std::list<::std::shared_ptr<WindowDesc>> m_WindowHandles;

};


} // !Core

#endif // !AB_APPSTATUS_H
