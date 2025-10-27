SET(RENDERING_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Primitives/Object.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/Instance.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/WrapperHardware.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/WrapperAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/MinimalHardware.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/RTXHardware.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/ComputeAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/RTXDeviceAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Vulkan/SwapChain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/Rays.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/VoxelPipeline.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/VoxelGrid.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Raycaster/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Voxels.cpp"
)
SET(RENDERING_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Consts.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Mat4.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Vec3.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Vec4.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Rot.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Operations.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Primitives/Object.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Primitives/Camera.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/ErrorHandling.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/GPUBuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/Instance.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/MinimalHardware.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/RTXHardware.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/ComputeAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/RTXDeviceAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/SwapChain.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Vulkan/FrameResources.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/VoxelPipeline.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/PushConstants.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/SingleVoxel.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/VoxelGrid.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/Renderer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Raycaster/Rays.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Voxels.hpp"
)

ADD_LIBRARY(RenderingLib SHARED 
    "${RENDERING_SOURCE}"
    "${RENDERING_HEADRES}"
)

FIND_PACKAGE(Vulkan REQUIRED)

TARGET_INCLUDE_DIRECTORIES(RenderingLib PUBLIC 
    "${Vulkan_INCLUDE_DIRS}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_LINK_LIBRARIES(RenderingLib PUBLIC 
    "${Vulkan_LIBRARIES}"
    CoreLib
    AppLib
)

TARGET_PRECOMPILE_HEADERS(RenderingLib PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/Voxels.hpp")

TARGET_COMPILE_DEFINITIONS(RenderingLib PRIVATE _BEAST_EXPORTS)

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/shaders.cmake)
compileShaders("${CMAKE_CURRENT_SOURCE_DIR}/Public/Shaders" CompileShadersForVoxels)
ADD_DEPENDENCIES(RenderingLib CompileShadersForVoxels)
