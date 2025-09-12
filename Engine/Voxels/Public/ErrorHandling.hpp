#ifndef AB_ERROR_HANDLING_H
#define AB_ERROR_HANDLING_H

#include "Voxels.hpp"

namespace Voxels
{

constexpr inline void ThrowIfFailed(VkResult result,
                                    wchar_t pwszCustomLogMsg[] = L"Ohh nooo... Vulkan isn't working!!! "
                                                                 L"Error code is: %d")
{
    if (result != VK_SUCCESS) {
        AB_LOG(Core::Debug::Error, pwszCustomLogMsg, result);
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }
}

} // !Voxels

#endif // !AB_ERROR_HANDLING_H
