#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Raycaster/Renderer.hpp"
#include "Synchronization/DeltaTime.hpp"

#include "CameraController.hpp"
#include "Synchronization/FpsLimiter.hpp"
#include "Window/WindowPolicy/BasicSystemPolicy.hpp"
#include "Window/WindowPolicy/GameSystemPolicy.hpp"

using namespace Core;
using namespace App;

int main()
{
    EmptyCanvas renderWindow;
    const auto& input = renderWindow.GetInput();
    Voxels::Renderer render = { };
    DeltaTime dt = { };
    FpsLimiter fl(1000.f / 60.f);


    ::std::shared_ptr<PlayablePaper> pwc = ::std::make_shared<PlayablePaper>();
	const auto& pc = pwc->GetCharacter();

    // Set up
	renderWindow.GetWindowDesc()->Name = L"Voxels Raytracing Demo!";
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
    dt.SetReferenceFrame();
    while (AppStatus::GetAppCurrentStatus()) 
    {   
        const float fDeltaMs = dt.FetchMs();
        fl.Block(fDeltaMs);
        // AB_LOG(Core::Debug::Info, 
        //        L"Fps: %f Frame duration: %fms",
        //        1000.f / fDeltaMs,
        //        fDeltaMs);

        renderWindow.Update(fDeltaMs);
        render.Update(fDeltaMs);
        render.Render();
    }

    AB_LOG(Core::Debug::Info, L"App is closing...");

    render.Destroy();

    // Core::Debug::Logger::Get()->~Logger();
}
