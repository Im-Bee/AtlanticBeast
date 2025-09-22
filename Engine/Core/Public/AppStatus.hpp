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

    AppStatus();

    static EAppStatus m_AppCurrentStatus;

public:

    static AppStatus& Get();

    ~AppStatus();

public:

    static EAppStatus GetAppCurrentStatus();

private:

    uint32_t SendClosedWindowSignal();

    uint32_t SendOpenedWindowSignal();

    void UpdateStatus();

private:

    size_t m_NumberOfWindows;

};


} // !Core

#endif // !AB_APPSTATUS_H
