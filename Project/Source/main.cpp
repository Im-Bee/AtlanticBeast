#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Renderer.hpp"

int main()
{
	Core::EmptyCanvas window = { };
	Core::EmptyCanvas window2(L"That other window");
    Core::UserInput& input = window.GetInput();
    Core::UserInput& input2 = window2.GetInput();
    Voxels::Renderer render = { };


    window.Create();
    window2.Create();

    input.AddHandler();
    input.StartCapturing();

    input2.StartCapturing();

    render.Initialize(window.GetWindowDesc());

    while (Core::AppStatus::GetAppCurrentStatus()) {
		// AB_LOG(Core::Debug::Info, L"App is running...");
        window.Update();
        window2.Update();
        input.Update();
	    render.Render();
    }

	AB_LOG(Core::Debug::Info, L"App is closing...");

    window.Destroy();
}
