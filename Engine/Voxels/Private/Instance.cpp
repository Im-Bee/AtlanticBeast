#include "Instance.hpp"
#include <vulkan/vulkan_core.h>

namespace Voxels
{

using namespace std;

// Instance // ---------------------------------------------------------------------------------------------------------
Instance::Instance()
    : m_Instance(CreateInstance())
{ }

// ---------------------------------------------------------------------------------------------------------------------
Instance::~Instance()
{ 
    if (m_Instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_Instance, NULL);
        m_Instance = VK_NULL_HANDLE;
    }
}

// Private // ----------------------------------------------------------------------------------------------------------
VkInstance Instance::CreateInstance()
{ 
    VkInstance              instance;
    VkApplicationInfo       appInfo;
    VkInstanceCreateInfo    createInfo;
    VkResult                result;

    const vector<const char*> vpszValidationLayers = {
#ifdef _DEBUG
        "VK_LAYER_KHRONOS_validation",
#endif // !_DEBUG
    };

    const vector<const char*> vpszExtensions = {
        "VK_KHR_surface",
#ifdef _WIN32
        "VK_KHR_win32_surface"
#elif __linux__
        "VK_KHR_xlib_surface"
#endif // !_WIN32
    };

    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext               = NULL;
    appInfo.pApplicationName    = "Voxels";
    appInfo.applicationVersion  = 0x00000001;
    appInfo.pEngineName         = "AtlanticBeast";
    appInfo.engineVersion       = 0x00000010;
    appInfo.apiVersion          = VK_API_VERSION_1_4;


    createInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext                    = NULL;
    createInfo.flags                    = 0;
    createInfo.pApplicationInfo         = &appInfo;
    createInfo.ppEnabledLayerNames      = !vpszValidationLayers.empty() ? &vpszValidationLayers[0] : nullptr;
    createInfo.enabledLayerCount        = vpszValidationLayers.size();
    createInfo.ppEnabledExtensionNames  = !vpszExtensions.empty() ? &vpszExtensions[0] : nullptr;
    createInfo.enabledExtensionCount    = vpszExtensions.size();

    result = vkCreateInstance(&createInfo,
                              NULL,
                              &instance);

    if (result != VK_SUCCESS) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!! Error code is: %d", result);
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }

    return instance;
}

} // !Core::Voxels

