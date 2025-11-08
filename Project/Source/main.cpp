#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Raycaster/Renderer.hpp"
#include "Synchronization/DeltaTime.hpp"
#include "Synchronization/FpsLimiter.hpp"

#include "CameraController.hpp"
#include "Game.hpp"

using namespace std;
using namespace Core;
using namespace App;

int main()
{
    EmptyCanvas renderWindow;
    const auto& input = renderWindow.GetInput();
    Voxels::Renderer render = { };
    Game g = { };
    DeltaTime dt = { };
    FpsLimiter fl(1000.f / 120.f);


    ::std::shared_ptr<PlayablePaper> pwc = ::std::make_shared<PlayablePaper>();
	const auto& pc = pwc->GetCharacter();

    // Set up
	renderWindow.GetWindowDesc()->Name = L"Voxels Raytracing Demo!";
    renderWindow.Create();
    pwc->BindToInput(input);

    input->StartCapturing();

    render.SetCurrentCamera(::std::dynamic_pointer_cast<Voxels::Camera>(pc));
    render.Initialize(renderWindow.GetWindowDesc(), g.GetWorld());
    
    pc->SetRotation(Voxels::Vec3 { -0.5f, 1.25f, 0.f });
    pc->SetPositon(Voxels::Vec3 { 14.5f, 2.25f, 25.f });
    pc->SetGrid(g.GetWorld()); 

	// Main loop
    dt.SetReferenceFrame();
    while (AppStatus::GetAppCurrentStatus()) 
    {   
        const float fDeltaMs = dt.FetchMs();
        renderWindow.Update(fDeltaMs);
        render.Update(fDeltaMs);
        render.Render();
        const float fBlock = fl.Block(dt.DeltaMs(), fDeltaMs);


        // ::Core::Debug::Logger::Get()->Log(::Core::Debug::Info, 
        //                                   L"Fps: %f Frame duration: %fms Blocked for: %fms",
        //                                   1000.f / fDeltaMs,
        //                                   fDeltaMs,
        //                                   fBlock);
    }

    AB_LOG(Core::Debug::Info, L"App is closing...");

    render.Destroy();

    // Core::Debug::Logger::Get()->~Logger();
}
