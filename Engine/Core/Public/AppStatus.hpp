#ifndef AB_APPSTATUS_H
#define AB_APPSTATUS_H

#include "ExportImport.h"
#include "CSystem.hpp"

namespace Core
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

public:
    static AppStatus& Get();

    ~AppStatus();

    static EAppStatus GetAppCurrentStatus();

private:

    uint32_t SendClosedWindowSignal();
    uint32_t SendOpenedWindowSignal();

    void UpdateStatus();

    size_t m_NumberOfWindows;

    AppStatus();

    static EAppStatus m_AppCurrentStatus;
};


} // !Core

#endif // !AB_APPSTATUS_H
