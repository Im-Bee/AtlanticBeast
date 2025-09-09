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

    while (Core::AppStatus::GetAppCurrentStatus()) {
        iec.Update();
    }

    iec.Destroy();
}
