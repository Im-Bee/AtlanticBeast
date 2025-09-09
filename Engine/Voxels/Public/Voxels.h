#ifndef AB_VOXELS_H
#define AB_VOXELS_H

#ifdef _WIN32
#   define VK_USE_PLATFORM_WIN32_KHR
#elif __linux
#   define VK_USE_PLATFORM_XLIB_KHR
#endif

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

#include "Core.h"

#endif // !AB_VOXELS_H
