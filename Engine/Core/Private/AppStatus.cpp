#include "Core.h"

#include "AppStatus.hpp"

namespace Core
{

EAppStatus AppStatus::m_AppCurrentStatus = EAppStatus::Dead;

AppStatus::AppStatus():
    m_NumberOfWindows(0)
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

void AppStatus::SendClosedWindowSignal()
{
    AB_LOG(Debug::Info, L"Got close window signal");

    if (m_NumberOfWindows > 0)
    {
        --m_NumberOfWindows;
    }

    UpdateStatus();
}

void AppStatus::SendOpenedWindowSignal()
{
    AB_LOG(Debug::Info, L"Got new window signal");

    ++m_NumberOfWindows;

    UpdateStatus();
}

void AppStatus::UpdateStatus()
{
	AB_LOG(Debug::ESeverity::Info, L"AppStatus: Number of windows: %d", m_NumberOfWindows);

    if (m_NumberOfWindows == 0)
    {
        m_AppCurrentStatus = EAppStatus::Dead;
        return;
    }

    m_AppCurrentStatus = EAppStatus::Running;
}

} // !Core
