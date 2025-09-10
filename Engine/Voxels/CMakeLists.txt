SET(VOXELS_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Instance.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Hardware.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/DeviceAdapter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/SwapChain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Voxels.cpp"
)
SET(VOXELS_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Instance.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Hardware.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/DeviceAdapter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/SwapChain.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Renderer.hpp"
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
