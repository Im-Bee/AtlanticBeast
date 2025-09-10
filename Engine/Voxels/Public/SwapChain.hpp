#ifndef AB_SWAPCHAIN_H
#define AB_SWAPCHAIN_H

#include "Voxels.h"
#include "WindowDesc.h"
#include "Instance.hpp"

namespace Voxels
{

class Swapchain
{
public:

    Swapchain(Instance* inst, const Core::WindowDesc& wd);

    ~Swapchain();

private:

    VkSurfaceKHR CreateSurface(const Core::WindowDesc& wd);

private:

    Instance*       m_pInstance     = nullptr;

    VkSurfaceKHR    m_Surface       = VK_NULL_HANDLE;
    VkSwapchainKHR  m_SwapChain     = VK_NULL_HANDLE;

};

} // !Voxels
#endif // !AB_SWAPCHAIN_H
