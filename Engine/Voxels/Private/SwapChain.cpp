#include "SwapChain.hpp"


namespace Voxels
{

using namespace std;

// Swapchain // ---------------------------------------------------------------------------------------------------------
Swapchain::Swapchain(::std::shared_ptr<Instance> pInst, const Core::WindowDesc& wd)
    : m_pInstance(pInst)
    , m_Surface(CreateSurface(wd))
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
VkSurfaceKHR Swapchain::CreateSurface(const Core::WindowDesc& wd)
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
    createInfo.dpy      = wd.Display;
    createInfo.window   = wd.Window;

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

} // !Voxels
