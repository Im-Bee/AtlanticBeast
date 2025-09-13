#include "Core.h"
#include "EmptyCanvas.hpp"
#include "Renderer.hpp"

int main()
{
	Core::EmptyCanvas iec;

    iec.Create();

    ::std::shared_ptr<Voxels::Instance>        inst = ::std::make_shared<Voxels::Instance>();
    ::std::shared_ptr<Voxels::Hardware>        hw   = ::std::make_shared<Voxels::Hardware>(inst);
    ::std::shared_ptr<Voxels::DeviceAdapter>   da   = ::std::make_shared<Voxels::DeviceAdapter>(hw);
    ::std::shared_ptr<Voxels::Swapchain>       sw   = ::std::make_shared<Voxels::Swapchain>(inst,
                                                                                            hw,
                                                                                            da,
                                                                                            iec.GetWindowDesc());

    ::std::shared_ptr<Voxels::Pipeline>        pl   = ::std::make_shared<Voxels::Pipeline>(da);

    while (Core::AppStatus::GetAppCurrentStatus()) {
		// AB_LOG(Core::Debug::Info, L"App is running...");
        iec.Update();
    }

	AB_LOG(Core::Debug::Info, L"App is closing...");

    iec.Destroy();
}
