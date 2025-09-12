#ifndef AB_VOXELS_H
#define AB_VOXELS_H

#ifdef _WIN32
#   define VK_USE_PLATFORM_WIN32_KHR
#elif __linux
#   define VK_USE_PLATFORM_XLIB_KHR
#endif

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "Core.h"

#ifdef __cplusplus

#include <vulkan/vulkan.hpp>

#endif // !__cplusplus

#ifdef __linux__
#   ifdef _BEAST_VOXELS_EXPORTS
#         define BEAST_VOXEL_API __attribute__((visibility("default")))
#      else
#         define BEAST_VOXEL_API
#   endif // !_BEAST_EXPORTS
#elif _WIN32
#   ifdef _BEAST_VOXELS_EXPORTS
#           define BEAST_VOXEL_API __declspec(dllexport)
#       else
#           define BEAST_VOXEL_API __declspec(dllimport)
#   endif // !_BEAST_EXPORTS
#endif // !__linux__

#endif // !AB_VOXELS_H
