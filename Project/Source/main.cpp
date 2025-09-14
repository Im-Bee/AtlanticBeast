#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Renderer.hpp"

int main()
{
	Core::EmptyCanvas iec = { };
    Voxels::Renderer ren = { };

    iec.Create();
    iec.Update();
    ren.Initialize(iec.GetWindowDesc());

    while (Core::AppStatus::GetAppCurrentStatus()) {
		// AB_LOG(Core::Debug::Info, L"App is running...");
        iec.Update();
	    ren.Render();
    }

	AB_LOG(Core::Debug::Info, L"App is closing...");

    iec.Destroy();
}
