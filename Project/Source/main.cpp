#include "Core.h"
#include "EmptyCanvas.hpp"
#include "EmptyWindow.hpp"
#include "Raycaster/Renderer.hpp"
#include "Math/Operations.hpp"

#include "CameraController.hpp"

int main()
{
    Core::EmptyCanvas renderWindow = { };
    Core::UserInput& input = renderWindow.GetInput();
    auto windowDesc1 = renderWindow.GetWindowDesc();
    Voxels::Renderer render = { };

    // Set up
    {
        renderWindow.Create();

        ::std::shared_ptr<PlayablePaper> pwc = ::std::make_shared<PlayablePaper>();

        Core::InputBind ib;
        ib.Type     = Core::EBindType::Keyboard;
        ib.keyboard = Core::KeyboardBind { Core::EOnKeyState::Continuous, Core::AB_KEY_A };

        input.Bind(pwc.get(), &PlayablePaper::UseActionMoveLeft, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_D;
        input.Bind(pwc.get(), &PlayablePaper::UseActionMoveRight, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_W;
        input.Bind(pwc.get(), &PlayablePaper::UseActionMoveFront, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_S;
        input.Bind(pwc.get(), &PlayablePaper::UseActionMoveBack, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_Q;
        input.Bind(pwc.get(), &PlayablePaper::UseActionMoveUp, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_E;
        input.Bind(pwc.get(), &PlayablePaper::UseActionMoveDown, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_1;
        input.Bind(pwc.get(), &PlayablePaper::UseActionLeftYaw, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_2;
        input.Bind(pwc.get(), &PlayablePaper::UseActionRightYaw, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_3;
        input.Bind(pwc.get(), &PlayablePaper::UseActionUpPitch, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_4;
        input.Bind(pwc.get(), &PlayablePaper::UseActionDownPitch, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_5;
        input.Bind(pwc.get(), &PlayablePaper::UseActionFovUp, ib);

        ib.keyboard.KeyCode = Core::AB_KEY_6;
        input.Bind(pwc.get(), &PlayablePaper::UseActionFovDown, ib);

        input.StartCapturing();

        pwc->SetRotation(Voxels::Vec3 { -0.5, 1.25, 0. });
        pwc->SetPositon(Voxels::Vec3 { 14.5, 10.25, 25. });

        render.SetCurrentCamera(::std::dynamic_pointer_cast<Voxels::Camera>(pwc));
        render.Initialize(renderWindow.GetWindowDesc());
    }
    
    auto vg = render.GetGrid();
    while (Core::AppStatus::GetAppCurrentStatus()) 
    {
        renderWindow.Update();
        input.Update();
        
        if (windowDesc1->IsAlive) 
        {
            static uint8_t r = 0;
            static uint8_t g = 111;
            static uint8_t b = 52;
            static int32_t index = 0; 
                                   
            if (index < vg->GetAmountOfVoxels()) {
                Voxels::Voxel v;
                v.Type = 1;
                v.RGBA = (static_cast<uint32_t>(r++) << 24) |
                    (static_cast<uint32_t>(g++) << 16) |
                    (static_cast<uint32_t>(b++) << 8) | + 0x000000FF;
        
                vg->ModifyVoxel(index++, std::move(v));
            }

            render.Update();
            render.Render();
        }
    }

    AB_LOG(Core::Debug::Info, L"App is closing...");

    render.Destroy();

    // Core::Debug::Logger::Get()->~Logger();
}
