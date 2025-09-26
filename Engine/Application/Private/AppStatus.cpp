#include "App.h"
#include "App.h"

#include "AppStatus.hpp"

namespace App
{

using namespace Core;

EAppStatus AppStatus::m_AppCurrentStatus = EAppStatus::Dead;

AppStatus::AppStatus():
    m_uNumberOfWindows(0)
{ }

AppStatus::~AppStatus()
{ }

AppStatus& AppStatus::Get()
{
	static AppStatus instance;
	return instance;
}

EAppStatus AppStatus::GetAppCurrentStatus()
{
	return m_AppCurrentStatus;
}

uint32_t AppStatus::SendClosedWindowSignal()
{
    AB_LOG(Debug::Info, L"Got close window signal");

    if (m_uNumberOfWindows > 0)
    {
        --m_uNumberOfWindows;
    }

    UpdateStatus();

    return m_uNumberOfWindows;
}

uint32_t AppStatus::SendOpenedWindowSignal()
{
    AB_LOG(Debug::Info, L"Got new window signal");

    ++m_uNumberOfWindows;

    UpdateStatus();

    return m_uNumberOfWindows;
}

void AppStatus::UpdateStatus()
{
	AB_LOG(Debug::ESeverity::Info, L"AppStatus: Number of windows: %d", m_uNumberOfWindows);

    if (m_uNumberOfWindows == 0)
    {
        m_AppCurrentStatus = EAppStatus::Dead;
        return;
    }

    m_AppCurrentStatus = EAppStatus::Running;
}

} // !App
