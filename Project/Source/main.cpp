#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Renderer.hpp"

int main()
{
	Core::IBaseWindow iec(Core::WindowDesc(L"First"));

    iec.Create();

    Voxels::Instance        inst;
    Voxels::Hardware        hw(inst);
    Voxels::DeviceAdapter   da(hw);
    Voxels::Swapchain       sw(&inst, iec.GetWindowDesc());

    while (Core::AppStatus::GetAppCurrentStatus()) {
        iec.Update();
    }

    iec.Destroy();
}
