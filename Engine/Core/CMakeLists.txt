SET(CORE_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Exception.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Debug/Logger.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Synchronization/DeltaTime.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Synchronization/FpsLimiter.cpp"
)
SET(CORE_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AbLimits.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CStandard.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CPlusPlusStandard.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Exception.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/ExportImport.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/SystemIncludes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Debug/Assert.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Debug/Logger.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Synchronization/FpsLimiter.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Synchronization/DeltaTime.hpp"
)

ADD_LIBRARY(CoreLib SHARED
    "${CORE_SOURCE}"
    "${CORE_HEADRES}"
)

TARGET_INCLUDE_DIRECTORIES(CoreLib PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_PRECOMPILE_HEADERS(CoreLib PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/Core.h")

TARGET_COMPILE_DEFINITIONS(CoreLib PRIVATE _BEAST_EXPORTS)

