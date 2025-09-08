#include "Core.h"
#include "EmptyCanvas.hpp"

int main()
{
    Core::Debug::Logger::Get();
    Core::AppStatus::Get();

	Core::EmptyCanvas iec(L"First");
	Core::EmptyCanvas iecSecond(L"Second");

    iec.Create();
	iecSecond.Create();

    while (Core::AppStatus::Get().GetAppCurrentStatus()) {
        iec.Update();
        iecSecond.Update();
    }

    iec.Destroy();
	iecSecond.Destroy();

    delete &Core::AppStatus::Get();
    delete Core::Debug::Logger::Get();
}
