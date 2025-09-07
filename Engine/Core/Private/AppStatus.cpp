#include "Core.h"

namespace Core
{

AppStatus* AppStatus::m_pInstance = new AppStatus();
EAppStatus AppStatus::m_AppCurrentStatus = EDead;

AppStatus::AppStatus()
{
}

AppStatus::~AppStatus()
{
	delete m_pInstance;
}

AppStatus& AppStatus::Get()
{
	return *m_pInstance;
}

EAppStatus AppStatus::GetAppCurrentStatus() const
{
	return m_AppCurrentStatus;
}

} // !Core
