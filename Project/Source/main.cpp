#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Raycaster/Renderer.hpp"
#include "Math/Operations.hpp"

#include "CameraController.hpp"

int main()
{
    Core::EmptyCanvas window = { };
    Core::UserInput& input = window.GetInput();
    auto windowDesc1 = window.GetWindowDesc();

    Voxels::Renderer render = { };

    window.Create();

    PaperWeightController pwc;
    Core::InputBind ib;
    ib.Type = Core::EBindType::Keyboard;
    ib.keyboard = Core::KeyboardBind { Core::EKeyState::Press, Core::AB_KEY_A };

    input.Bind(&pwc, &PaperWeightController::UseActionMove, ib);
    input.StartCapturing();
    
    render.Initialize(window.GetWindowDesc());
    
    while (Core::AppStatus::GetAppCurrentStatus()) 
    {
        window.Update();
        input.Update();
        
        if (windowDesc1->IsAlive) {
            render.Update();
            render.Render();
        }
    }

    AB_LOG(Core::Debug::Info, L"App is closing...");

    render.Destroy();

    // Core::Debug::Logger::Get()->~Logger();
}
