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
	delete m_pInstance;
}

AppStatus& AppStatus::Get()
{
	return *m_pInstance;
}

EAppStatus AppStatus::GetAppCurrentStatus()
{
	return m_AppCurrentStatus;
}

} // !Core
