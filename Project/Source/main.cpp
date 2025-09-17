#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Renderer.hpp"

int main()
{
    Core::EmptyCanvas window = { };
    Core::UserInput& input = window.GetInput();
    auto windowDesc1 = window.GetWindowDesc();

    Core::EmptyCanvas window2(L"That other window");
    Core::UserInput& input2 = window2.GetInput();
    auto windowDesc2= window2.GetWindowDesc();

    Voxels::Renderer render = { };

    window2.Create();
    input2.StartCapturing();

    window.Create();
    input.StartCapturing();

    render.Initialize(window.GetWindowDesc());
    
    while (Core::AppStatus::GetAppCurrentStatus()) {
        if (windowDesc2->IsAlive) {
            window2.Update();
            input2.Update();
        }

        if (windowDesc1->IsAlive) {
            render.Render();
            window.Update();
            input.Update();
        }
    }

    AB_LOG(Core::Debug::Info, L"App is closing...");
}
