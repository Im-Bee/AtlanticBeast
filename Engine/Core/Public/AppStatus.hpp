#ifndef AB_APPSTATUS_H
#define AB_APPSTATUS_H

#include "Core.h"

namespace Core
{

enum EAppStatus
{
	Dead,
	Running
};


class AppStatus
{
public:
    static AppStatus& Get();

    ~AppStatus();

	static EAppStatus GetAppCurrentStatus();

	void SendClosedWindowSignal();
	void SendOpenedWindowSignal();

private:

	void UpdateStatus();

	size_t m_NumberOfWindows;

	template<typename T>
	friend class IWindow;

	AppStatus();

	static AppStatus* m_pInstance;
	static EAppStatus m_AppCurrentStatus;
};


} // !Core

#endif // !AB_APPSTATUS_H
