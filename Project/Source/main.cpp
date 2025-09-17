#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Renderer.hpp"

int main()
{
	Core::EmptyCanvas window = { };
    Core::UserInput& input = window.GetInput();
    Voxels::Renderer render = { };


    window.Create();

    input.AddHandler();
    input.StartCapturing();

    // render.Initialize(window.GetWindowDesc());

    while (Core::AppStatus::GetAppCurrentStatus()) {
		// AB_LOG(Core::Debug::Info, L"App is running...");
        window.Update();
	    // render.Render();
    }

	AB_LOG(Core::Debug::Info, L"App is closing...");

    window.Destroy();
}
