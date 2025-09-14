#include "Instance.hpp"

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

// ---------------------------------------------------------------------------------------------------------------------
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData)
{
    constexpr wchar_t pwszFormat[] =
#ifdef _WIN32
        L"[Vulkan]: %S";
#elif __linux__
        L"[Vulkan]: %s";
#endif // !_WIN32

    AB_LOG(Core::Debug::Info, pwszFormat, pCallbackData->pMessage);
    return VK_FALSE;
}

// Private // ----------------------------------------------------------------------------------------------------------
VkInstance Instance::CreateInstance()
{ 
    VkInstance                          instance;
    VkApplicationInfo                   appInfo;
    VkInstanceCreateInfo                createInfo;
    VkResult                            result;

#ifdef _DEBUG
    VkDebugUtilsMessengerCreateInfoEXT  debugCreateInfo;

    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.pNext = NULL;
    debugCreateInfo.flags = 0;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback     = debugCallback;
    debugCreateInfo.pUserData           = NULL;
#endif // !_DEBUG


    const vector<const char*> vpszValidationLayers = {
#ifdef _DEBUG
        "VK_LAYER_KHRONOS_validation",
#endif // !_DEBUG
    };

    const vector<const char*> vpszExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif __linux__
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#endif // !_WIN32

#ifdef _DEBUG
        VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
    };

    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext               = NULL;
    appInfo.pApplicationName    = "Voxels";
    appInfo.applicationVersion  = VK_MAKE_VERSION(0, 1, 1);
    appInfo.pEngineName         = "AtlanticBeast";
    appInfo.engineVersion       = VK_MAKE_VERSION(0, 1, 1);
    appInfo.apiVersion          = VK_API_VERSION_1_3;


    createInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext                    = 
#ifdef _DEBUG
        &debugCreateInfo;
#else
        NULL;
#endif
    createInfo.flags                    = 0;
    createInfo.pApplicationInfo         = &appInfo;
    createInfo.ppEnabledLayerNames      = !vpszValidationLayers.empty() ? &vpszValidationLayers[0] : nullptr;
    createInfo.enabledLayerCount        = static_cast<uint32_t>(vpszValidationLayers.size());
    createInfo.ppEnabledExtensionNames  = !vpszExtensions.empty() ? &vpszExtensions[0] : nullptr;
    createInfo.enabledExtensionCount    = static_cast<uint32_t>(vpszExtensions.size());

    result = vkCreateInstance(&createInfo,
                              NULL,
                              &instance);

    if (result != VK_SUCCESS) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!! Error code is: %d", result);
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }

    return instance;
}

} // !Voxels

