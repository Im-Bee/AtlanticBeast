#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Renderer.hpp"

int main()
{
	Core::IBaseWindow iec(Core::WindowDesc(L"First"));

    iec.Create();

    ::std::shared_ptr<Voxels::Instance>        inst = ::std::make_shared<Voxels::Instance>();
    ::std::shared_ptr<Voxels::Hardware>        hw   = ::std::make_shared<Voxels::Hardware>(inst);
    ::std::shared_ptr<Voxels::DeviceAdapter>   da   = ::std::make_shared<Voxels::DeviceAdapter>(hw);
    ::std::shared_ptr<Voxels::Swapchain>       sw   = ::std::make_shared<Voxels::Swapchain>(inst, iec.GetWindowDesc());

    while (Core::AppStatus::GetAppCurrentStatus()) {
        iec.Update();
    }

    iec.Destroy();
}
