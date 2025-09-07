#include "Core.h"

int main()
{
    Core::Debug::Logger::Get();
    Core::AppStatus::Get();

    delete &Core::AppStatus::Get();
    delete Core::Debug::Logger::Get();
}
