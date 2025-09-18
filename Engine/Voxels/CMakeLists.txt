SET(VOXELS_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Instance.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Hardware.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/DeviceAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/SwapChain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/Pipeline.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/VoxelGrid.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Voxels.cpp"
)
SET(VOXELS_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Mat4.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Vec3.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Vec4.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/ErrorHandling.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Instance.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Hardware.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/DeviceAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/SwapChain.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/Pipeline.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/PushConstants.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/SingleVoxel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/VoxelGrid.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/Renderer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Voxels.hpp"
)

ADD_LIBRARY(VoxelsLib SHARED 
    "${VOXELS_SOURCE}"
    "${VOXELS_HEADRES}"
)

FIND_PACKAGE(Vulkan REQUIRED)

TARGET_INCLUDE_DIRECTORIES(VoxelsLib PUBLIC 
    "${Vulkan_INCLUDE_DIRS}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_LINK_LIBRARIES(VoxelsLib PUBLIC 
    "${Vulkan_LIBRARIES}"
    CoreLib
)

TARGET_PRECOMPILE_HEADERS(VoxelsLib PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/Voxels.hpp")

TARGET_COMPILE_DEFINITIONS(VoxelsLib PRIVATE _BEAST_VOXELS_EXPORTS)
