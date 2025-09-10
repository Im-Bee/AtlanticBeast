#include "SwapChain.hpp"


namespace Voxels
{

using namespace std;

// Swapchain // --------------------------------------------------------------------------------------------------------
Swapchain::Swapchain(::std::shared_ptr<Instance> pInst, 
                     ::std::shared_ptr<Hardware> hw,
                     ::std::shared_ptr<const WindowDesc> wd)
    : m_pInstance(pInst)
    , m_pHardware(hw)
    , m_pWindiowdesc(wd)
    , m_Surface(CreateSurface())
    , m_SwapChain(CreateSwapChain())
{ }

// ---------------------------------------------------------------------------------------------------------------------
Swapchain::~Swapchain()
{
    if (m_Surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(m_pInstance->GetInstance(), m_Surface, nullptr);
        m_Surface = VK_NULL_HANDLE;
    }
    m_pInstance = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
VkSurfaceKHR Swapchain::CreateSurface()
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkResult     result;

#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext     = NULL;
    createInfo.flags     = 0;
    createInfo.hinstance = GetModuleHandle(NULL);
    createInfo.hwnd      = wd.Hwnd;

    result = vkCreateWin32SurfaceKHR(m_pInstance->GetInstance(),
                                     &createInfo,
                                     nullptr,
                                     &surface);
#elif __linux__
    VkXlibSurfaceCreateInfoKHR  createInfo;
    createInfo.sType    = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext    = NULL;
    createInfo.flags    = 0;
    createInfo.dpy      = m_pWindiowdesc->DisplayHandle;
    createInfo.window   = m_pWindiowdesc->WindowHandle;

    result = vkCreateXlibSurfaceKHR(m_pInstance->GetInstance(),
                                    &createInfo,
                                    NULL,
                                    &surface);
#endif // !_WIN32
       
    if (result != VK_SUCCESS) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!! Error code is: %d", result);
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }

    return surface;
}

// ---------------------------------------------------------------------------------------------------------------------
VkSwapchainKHR Swapchain::CreateSwapChain()
{
    VkSwapchainKHR              swapChain       = VK_NULL_HANDLE;
    VkSurfaceCapabilitiesKHR    capabilities;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_pHardware->GetPhysicalDevice(), m_Surface, &capabilities);

    return swapChain;
}

} // !Voxels
