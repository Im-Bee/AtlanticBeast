#ifndef AB_APPSTATUS_H
#define AB_APPSTATUS_H

#include "Core.h"

namespace Core
{

enum EAppStatus
{
	EDead,
	ERunning
};

class AppStatus
{
public:
	static AppStatus& Get();

	EAppStatus GetAppCurrentStatus() const;

private:
	AppStatus();
	~AppStatus();

	static AppStatus* m_pInstance;
	static EAppStatus m_AppCurrentStatus;
};


} // !Core

#endif // !AB_APPSTATUS_H