#ifndef AB_SWAPCHAIN_H
#define AB_SWAPCHAIN_H

#include "Voxels.hpp"
#include "Instance.hpp"
#include "Hardware.hpp"
#include "DeviceAdapter.hpp"
#include "Window/WindowDesc.h"

namespace Voxels
{

class BEAST_VOXEL_API Swapchain
{

	friend class Renderer;

public:
    
    static constexpr VkFormat TargetedFormat = VK_FORMAT_R8G8B8A8_UNORM;

public:

    Swapchain(::std::shared_ptr<const Instance> inst,
              ::std::shared_ptr<const Hardware> hw,
              ::std::shared_ptr<const DeviceAdapter> da,
              ::std::shared_ptr<const WindowDesc> wd);

    ~Swapchain();

public:

    VkSwapchainKHR GetSwapChainHandle() const
    { return m_SwapChain; }

    VkImage GetImage(uint32_t i) const
    { 
        if (i >= m_SwapChainImages.size()) {
            throw AB_EXCEPT("Getting VkImage out of bounds!");
        }

        return m_SwapChainImages[i]; 
    }

private:
    
    VkSurfaceKHR CreateSurface(::std::shared_ptr<const Instance>& pInstance,
                               ::std::shared_ptr<const WindowDesc>& pWindowDesc);

    VkSurfaceCapabilitiesKHR GetCapabilitesInternal(::std::shared_ptr<const Hardware> pHardware, VkSurfaceKHR surface);

    VkExtent2D GetExtentInternal(VkSurfaceCapabilitiesKHR& capabilities, 
                                 ::std::shared_ptr<const WindowDesc> pWindowDesc);
    
    uint32_t GetImageCountInternal(VkSurfaceCapabilitiesKHR& capabilities);

    VkSurfaceFormatKHR PickFormat(::std::shared_ptr<const Hardware>& pHardware, VkSurfaceKHR surface);

    VkPresentModeKHR PickMode(::std::shared_ptr<const Hardware>& pHardware, VkSurfaceKHR surface);

    VkSwapchainKHR CreateSwapChain(::std::shared_ptr<const DeviceAdapter>& pAdapter,
                                   VkSurfaceKHR surface,
                                   VkSurfaceCapabilitiesKHR& capabilities,
                                   VkExtent2D& extent2D,
                                   uint32_t uImageCount,
                                   VkSurfaceFormatKHR& surfaceFormat,
                                   VkPresentModeKHR presentMode);

    uint32_t CreateAmountOfSwapChainImages(::std::shared_ptr<const DeviceAdapter>& pAdapter, VkSwapchainKHR swapchain);

    ::std::vector<VkImage> CreateSwapChainImages(::std::shared_ptr<const DeviceAdapter>& pAdapter,
                                                 VkSwapchainKHR swapchain,
                                                 uint32_t uAmount);

private:

    ::std::shared_ptr<const Instance>       m_pInstance         = nullptr;
    ::std::shared_ptr<const Hardware>       m_pHardware         = nullptr;
    ::std::shared_ptr<const DeviceAdapter>  m_pDeviceAdapter    = nullptr;
    ::std::shared_ptr<const WindowDesc>     m_pWindowDesc       = nullptr;

    VkSurfaceKHR                m_Surface       = VK_NULL_HANDLE;
    VkSurfaceCapabilitiesKHR    m_Capabilities;
    VkExtent2D                  m_Extent;
    const uint32_t              m_uImageCount;
    VkSurfaceFormatKHR          m_SurfaceFormat;
    VkPresentModeKHR            m_PresentMode;
    VkSwapchainKHR              m_SwapChain     = VK_NULL_HANDLE;

    uint32_t                m_uCurrentImageIndex = 0;
    ::std::vector<VkImage>  m_SwapChainImages;

 };

} // !Voxels
#endif // !AB_SWAPCHAIN_H
