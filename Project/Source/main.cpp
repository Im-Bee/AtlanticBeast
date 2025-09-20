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

    ::std::shared_ptr<PlayablePaper> pwc = ::std::make_shared<PlayablePaper>();

    Core::InputBind ib;
    ib.Type     = Core::EBindType::Keyboard;
    ib.keyboard = Core::KeyboardBind { Core::EKeyState::Press, Core::AB_KEY_A };

    input.Bind(pwc.get(), &PlayablePaper::UseActionMoveLeft, ib);

    ib.keyboard.KeyCode = Core::AB_KEY_D;
    input.Bind(pwc.get(), &PlayablePaper::UseActionMoveRight, ib);

    ib.keyboard.KeyCode = Core::AB_KEY_W;
    input.Bind(pwc.get(), &PlayablePaper::UseActionMoveFront, ib);

    ib.keyboard.KeyCode = Core::AB_KEY_S;
    input.Bind(pwc.get(), &PlayablePaper::UseActionMoveBack, ib);

    ib.keyboard.KeyCode = Core::AB_KEY_1;
    input.Bind(pwc.get(), &PlayablePaper::UseActionLeftYaw, ib);

    ib.keyboard.KeyCode = Core::AB_KEY_2;
    input.Bind(pwc.get(), &PlayablePaper::UseActionRightYaw, ib);

    ib.keyboard.KeyCode = Core::AB_KEY_3;
    input.Bind(pwc.get(), &PlayablePaper::UseActionUpPitch, ib);

    ib.keyboard.KeyCode = Core::AB_KEY_4;
    input.Bind(pwc.get(), &PlayablePaper::UseActionDownPitch, ib);

    input.StartCapturing();
    
    pwc->SetRotation(Voxels::Vec3 { 0.5, 0.5, 1. });

    render.SetCurrentCamera(::std::dynamic_pointer_cast<Voxels::Camera>(pwc));
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
