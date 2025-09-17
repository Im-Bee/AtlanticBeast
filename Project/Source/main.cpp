#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Renderer.hpp"

int main()
{
    Core::EmptyCanvas window = { };
    Core::UserInput& input = window.GetInput();
    Core::EmptyCanvas window2(L"That other window");
    Core::UserInput& input2 = window2.GetInput();
    Voxels::Renderer render = { };


    window.Create();

    input.AddHandler();
    input.StartCapturing();


    window2.Create();
    input2.StartCapturing();

    render.Initialize(window.GetWindowDesc());
    
    while (Core::AppStatus::GetAppCurrentStatus()) {
        // AB_LOG(Core::Debug::Info, L"App is running...");

        if (window2.GetWindowDesc()->IsAlive) {
            window2.Update();
        }

        if (window.GetWindowDesc()->IsAlive) {
            window.Update();
            input.Update();
            render.Render();
        }
    }

    AB_LOG(Core::Debug::Info, L"App is closing...");

    //window.Destroy();
}
