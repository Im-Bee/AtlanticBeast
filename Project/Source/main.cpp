#include "Core.h"
#include "App.h"
#include "Voxels.hpp"

#include "EmptyCanvas.hpp"
#include "EmptyWindow.hpp"
#include "Raycaster/Renderer.hpp"
#include "Math/Operations.hpp"

#include "CameraController.hpp"

using namespace Core;
using namespace App;

int main()
{
    EmptyCanvas renderWindow = { };
    auto input = renderWindow.GetInput();
    auto windowDesc1 = renderWindow.GetWindowDesc();
    Voxels::Renderer render = { };

    // Set up
    ::std::shared_ptr<PlayablePaper> pwc = ::std::make_shared<PlayablePaper>();

    {
        renderWindow.Create();
        pwc->SignObject(input);

        InputBind ib;
        ib.Type     = EBindType::Keyboard;
        ib.keyboard = KeyboardBind { EOnKeyState::Continuous, AB_KEY_A };

        input->Bind(pwc.get(), &PlayablePaper::UseActionMoveLeft, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_D;
        input->Bind(pwc.get(), &PlayablePaper::UseActionMoveRight, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_W;
        input->Bind(pwc.get(), &PlayablePaper::UseActionMoveFront, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_S;
        input->Bind(pwc.get(), &PlayablePaper::UseActionMoveBack, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_Q;
        input->Bind(pwc.get(), &PlayablePaper::UseActionMoveUp, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_E;
        input->Bind(pwc.get(), &PlayablePaper::UseActionMoveDown, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_1;
        input->Bind(pwc.get(), &PlayablePaper::UseActionLeftYaw, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_2;
        input->Bind(pwc.get(), &PlayablePaper::UseActionRightYaw, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_3;
        input->Bind(pwc.get(), &PlayablePaper::UseActionUpPitch, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_4;
        input->Bind(pwc.get(), &PlayablePaper::UseActionDownPitch, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_5;
        input->Bind(pwc.get(), &PlayablePaper::UseActionFovUp, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_6;
        input->Bind(pwc.get(), &PlayablePaper::UseActionFovDown, nullptr, ib);

        ib.keyboard.KeyCode = AB_KEY_SPACE;
        ib.keyboard.KeyState = Press;
        input->Bind(pwc.get(), &PlayablePaper::UseActionPlaceBlock, nullptr, ib);


        ib.Type = Mouse;
        input->Bind(pwc.get(), nullptr, &PlayablePaper::UseActionMouse, ib);

        input->StartCapturing();

        pwc->SetRotation(Voxels::Vec3 { -0.5, 1.25, 0. });
        pwc->SetPositon(Voxels::Vec3 { 14.5, 10.25, 25. });

        render.SetCurrentCamera(::std::dynamic_pointer_cast<Voxels::Camera>(pwc));
        render.Initialize(renderWindow.GetWindowDesc());
    }
    
    auto vg = render.GetGrid();
    pwc->SetGrid(vg); 
    while (AppStatus::GetAppCurrentStatus()) 
    {
        renderWindow.Update();
        input->Update();
        
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
