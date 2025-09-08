#include "Core.h"
#include "EmptyCanvas.hpp"

int main()
{
	Core::IBaseWindow iec(Core::WindowDesc(L"First"));
	Core::IBaseWindow iecSecond(Core::WindowDesc(L"Second"));

    iec.Create();
	iecSecond.Create();

    while (Core::AppStatus::GetAppCurrentStatus()) {
        iec.Update();
        iecSecond.Update();
    }

    iec.Destroy();
	iecSecond.Destroy();
}
