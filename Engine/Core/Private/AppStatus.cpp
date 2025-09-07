#include "Core.h"

namespace Core
{

AppStatus* AppStatus::m_pInstance = new AppStatus();
EAppStatus AppStatus::m_AppCurrentStatus = Dead;

AppStatus::AppStatus()
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

} // !Core
