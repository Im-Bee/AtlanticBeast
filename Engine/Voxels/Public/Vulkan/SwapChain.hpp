#ifndef AB_SWAPCHAIN_H
#define AB_SWAPCHAIN_H

#include "Voxels.hpp"
#include "Instance.hpp"
#include "RTXHardware.hpp"
#include "RTXDeviceAdapter.hpp"
#include "Window/WindowDesc.hpp"

namespace Voxels
{

class Swapchain
{

	friend class Renderer;

public:
    
    static constexpr VkFormat TargetedFormat = VK_FORMAT_B8G8R8A8_UNORM;

public:

    BEAST_VOXEL_API Swapchain() = default;

    BEAST_VOXEL_API explicit Swapchain(::std::shared_ptr<const Instance> inst,
                                       ::std::shared_ptr<const RTXHardware> hw,
                                       ::std::shared_ptr<const RTXDeviceAdapter> da,
                                       ::std::shared_ptr<const WindowDesc> wd);

    BEAST_VOXEL_API ~Swapchain();

public:

    VkSwapchainKHR GetSwapChainHandle() const
    { return m_pSwapChain; }

    VkImage GetImage(uint32_t i) const
    { 
        AB_ASSERT(i < m_SwapChainImages.size());
        return m_SwapChainImages[i]; 
    }

private:
    
    VkSurfaceKHR CreateSurface(::std::shared_ptr<const Instance>& pInstance,
                               ::std::shared_ptr<const WindowDesc>& pWindowDesc);

    VkSurfaceCapabilitiesKHR GetCapabilitesInternal(::std::shared_ptr<const RTXHardware> pHardware, VkSurfaceKHR surface);

    VkExtent2D GetExtentInternal(VkSurfaceCapabilitiesKHR& capabilities, 
                                 ::std::shared_ptr<const WindowDesc> pWindowDesc);
    
    uint32_t GetImageCountInternal(VkSurfaceCapabilitiesKHR& capabilities);

    VkSurfaceFormatKHR PickFormat(::std::shared_ptr<const RTXHardware>& pHardware, VkSurfaceKHR surface);

    VkPresentModeKHR PickMode(::std::shared_ptr<const RTXHardware>& pHardware, VkSurfaceKHR surface);

    VkSwapchainKHR CreateSwapChain(::std::shared_ptr<const RTXDeviceAdapter>& pAdapter,
                                   VkSurfaceKHR surface,
                                   VkSurfaceCapabilitiesKHR& capabilities,
                                   VkExtent2D& extent2D,
                                   uint32_t uImageCount,
                                   VkSurfaceFormatKHR& surfaceFormat,
                                   VkPresentModeKHR presentMode);

    uint32_t CreateAmountOfSwapChainImages(::std::shared_ptr<const RTXDeviceAdapter>& pAdapter, VkSwapchainKHR swapchain);

    ::std::vector<VkImage> CreateSwapChainImages(::std::shared_ptr<const RTXDeviceAdapter>& pAdapter,
                                                 VkSwapchainKHR swapchain,
                                                 uint32_t uAmount);

private:

    ::std::shared_ptr<const Instance>       m_pInstance         = nullptr;
    ::std::shared_ptr<const RTXHardware>       m_pHardware         = nullptr;
    ::std::shared_ptr<const RTXDeviceAdapter>  m_pDeviceAdapter    = nullptr;
    ::std::shared_ptr<const WindowDesc>     m_pWindowDesc       = nullptr;

    VkSurfaceKHR                m_Surface       = VK_NULL_HANDLE;
    VkSurfaceCapabilitiesKHR    m_Capabilities  = { };
    VkExtent2D                  m_Extent        = { 0, 0 };
    const uint32_t              m_uImageCount   = -1;
    VkSurfaceFormatKHR          m_SurfaceFormat = { };
    VkPresentModeKHR            m_PresentMode   = { };
    VkSwapchainKHR              m_pSwapChain    = VK_NULL_HANDLE;

    uint32_t                m_uCurrentImageIndex = 0;
    ::std::vector<VkImage>  m_SwapChainImages;

 };

} // !Voxels
#endif // !AB_SWAPCHAIN_H
