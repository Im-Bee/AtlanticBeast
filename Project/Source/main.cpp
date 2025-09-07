#include "Core.h"
#include "EmptyCanvas.hpp"

int main()
{
    Core::Debug::Logger::Get();
    Core::AppStatus::Get();

    Core::EmptyCanvas iec = { };

    iec.Create();

    while (Core::AppStatus::GetAppCurrentStatus()) {
        iec.Update();
    }

    iec.Destroy();

    delete &Core::AppStatus::Get();
    delete Core::Debug::Logger::Get();
}
