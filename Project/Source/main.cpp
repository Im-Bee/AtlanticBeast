#include "Core.h"

int main()
{
    Core::Debug::Logger::Get();
    Core::AppStatus::Get();

    AB_LOG(Core::Debug::ESeverity::Info, L"AppStatus = %d", Core::AppStatus::Get().GetAppCurrentStatus() );

    for(int i = 0 ; i < 3 ; ++i)
        Core::AppStatus::Get().SendOpenedWindowSignal();

    AB_LOG(Core::Debug::ESeverity::Info, L"AppStatus = %d", Core::AppStatus::Get().GetAppCurrentStatus());

    for (int i = 0; i < 20; ++i)
        Core::AppStatus::Get().SendClosedWindowSignal();

    AB_LOG(Core::Debug::ESeverity::Info, L"AppStatus = %d", Core::AppStatus::Get().GetAppCurrentStatus());

    delete &Core::AppStatus::Get();
    delete Core::Debug::Logger::Get();
}
