#include "Core.h"

int main()
{
    Core::Debug::Logger::Get();
    Core::AppStatus::Get();

    Core::EmptyCanvas iec = { };

    iec.Create();

    while (1) {
        iec.Update();
    }

    iec.Destroy();

    delete &Core::AppStatus::Get();
    delete Core::Debug::Logger::Get();
}
