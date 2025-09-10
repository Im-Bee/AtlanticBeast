#ifndef AB_SWAPCHAIN_H
#define AB_SWAPCHAIN_H

#include "Voxels.hpp"
#include "Instance.hpp"
#include "Hardware.hpp"
#include "WindowDesc.h"

namespace Voxels
{

class BEAST_VOXEL_API Swapchain
{
public:

    Swapchain(::std::shared_ptr<Instance> inst,
              ::std::shared_ptr<Hardware> hw,
              ::std::shared_ptr<const WindowDesc> wd);

    ~Swapchain();

private:

    VkSurfaceKHR CreateSurface();

    VkSwapchainKHR CreateSwapChain();

private:

    ::std::shared_ptr<Instance> m_pInstance = nullptr;
    ::std::shared_ptr<Hardware> m_pHardware = nullptr;
    ::std::shared_ptr<const WindowDesc> m_pWindiowdesc = nullptr;

    VkSurfaceKHR    m_Surface       = VK_NULL_HANDLE;
    VkSwapchainKHR  m_SwapChain     = VK_NULL_HANDLE;

};

} // !Voxels
#endif // !AB_SWAPCHAIN_H
