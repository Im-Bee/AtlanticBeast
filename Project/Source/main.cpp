#include "Core.h"

#include <iostream>                             

int main()
{
    Core::Debug::Logger::Get();

    AB_LOG(Core::Debug::ESeverity::EInfo, L"Test");

    delete Core::Debug::Logger::Get();
}
