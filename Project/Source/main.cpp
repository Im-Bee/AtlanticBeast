#include "Core.h"

#include <iostream>                             

int main()
{
    Core::Debug::Logger::Get();

    AB_LOG(Core::Debug::ESeverity::EInfo, L"Test");

    AB_LOG( Core::Debug::ESeverity::EInfo, 
            L"Application Status = %d (0 = DEAD, 1 = RUNNING)", 
            Core::AppStatus::Get().GetAppCurrentStatus());

    delete Core::Debug::Logger::Get();
}
