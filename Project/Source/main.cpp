#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Raycaster/Renderer.hpp"

#include "CameraController.hpp"

using namespace Core;
using namespace App;

int main()
{
    EmptyCanvas renderWindow;
    const auto& input = renderWindow.GetInput();
    Voxels::Renderer render = { };

    ::std::shared_ptr<PlayablePaper> pwc = ::std::make_shared<PlayablePaper>();
	const auto& pc = pwc->GetCharacter();

    // Set up
    renderWindow.Create();
    pwc->BindToInput(input);

    input->StartCapturing();

    render.SetCurrentCamera(::std::dynamic_pointer_cast<Voxels::Camera>(pc));
    render.Initialize(renderWindow.GetWindowDesc());
    
    auto vg = render.GetGrid();
    pc->SetRotation(Voxels::Vec3 { -0.5f, 1.25f, 0.f });
    pc->SetPositon(Voxels::Vec3 { 14.5f, 30.25f, 25.f });
    pc->SetGrid(vg); 

	// Main loop
    while (AppStatus::GetAppCurrentStatus()) 
    {
        renderWindow.Update();
        
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

    AB_LOG(Core::Debug::Info, L"App is closing...");

    render.Destroy();

    // Core::Debug::Logger::Get()->~Logger();
}
