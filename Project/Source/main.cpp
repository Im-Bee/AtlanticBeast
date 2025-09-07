#include "Core.h"

#include <iostream>                             

int main()
{
    Core::Debug::Logger::Get();

    AB_LOG(Core::Debug::ESeverity::EInfo, L"Test");

    throw Core::Exception("An excetpino here", __LINE__, __FILE__);

    delete Core::Debug::Logger::Get();
}
