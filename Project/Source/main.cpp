#include "Core.h"
#include "EmptyCanvas.hpp"

int main()
{
	Core::EmptyCanvas iec(L"First");
	Core::EmptyCanvas iecSecond(L"Second");

    iec.Create();
	iecSecond.Create();

    while (Core::AppStatus::GetAppCurrentStatus()) {
        iec.Update();
        iecSecond.Update();
    }

    iec.Destroy();
	iecSecond.Destroy();
}
