#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Renderer.hpp"

int main()
{
    Core::EmptyCanvas window = { };
    Core::UserInput& input = window.GetInput();
    auto windowDesc1 = window.GetWindowDesc();

    Voxels::Renderer render = { };

    window.Create();
    input.StartCapturing();

    render.Initialize(window.GetWindowDesc());
    
    while (Core::AppStatus::GetAppCurrentStatus()) {
        window.Update();
        input.Update();
        if (windowDesc1->IsAlive) {
            render.Update();
            render.Render();
        }
    }

    AB_LOG(Core::Debug::Info, L"App is closing...");
}
