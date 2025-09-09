#include "Hardware.hpp"

namespace Core::Voxels
{

// Hardware // ---------------------------------------------------------------------------------------------------------
Hardware::Hardware(VkInstance& instance)
    : m_DeviceHandle(ChooseGPU(instance))
{ }

// ---------------------------------------------------------------------------------------------------------------------
Hardware::~Hardware()
{ }

// ---------------------------------------------------------------------------------------------------------------------
VkPhysicalDevice* Hardware::ChooseGPU(VkInstance& instance)
{ 
    VkPhysicalDevice* physicalDevice;



    return physicalDevice;
}

} // !Core::Voxels
