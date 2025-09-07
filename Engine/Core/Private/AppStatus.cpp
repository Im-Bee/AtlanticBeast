#include "Core.h"

namespace Core
{

AppStatus* AppStatus::m_pInstance = new AppStatus();
EAppStatus AppStatus::m_AppCurrentStatus = EAppStatus::Dead;

AppStatus::AppStatus():
    m_NumberOfWindows(0)
{
}

AppStatus::~AppStatus()
{
	if (m_pInstance) {
        m_pInstance = nullptr;
    }
}

AppStatus& AppStatus::Get()
{

    if (!m_pInstance) {
        m_pInstance = new AppStatus();
    }

	return *m_pInstance;
}

EAppStatus AppStatus::GetAppCurrentStatus()
{
	return m_AppCurrentStatus;
}

void AppStatus::SendClosedWindowSignal()
{
    if (m_NumberOfWindows > 0)
    {
        --m_NumberOfWindows;
    }

    UpdateStatus();
}

void AppStatus::SendOpenedWindowSignal()
{
    ++m_NumberOfWindows;

    UpdateStatus();
}

void AppStatus::UpdateStatus()
{
    if (m_NumberOfWindows == 0)
    {
        m_AppCurrentStatus = EAppStatus::Dead;
        return;
    }

    m_AppCurrentStatus = EAppStatus::Running;
}

} // !Core
