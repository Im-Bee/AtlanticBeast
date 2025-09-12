SET(CORE_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/WindowDesc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/AppStatus.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Exception.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/BaseWindowDetails.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/BaseWindowDetails.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Logger.cpp"
)
SET(CORE_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AbLimits.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AppStatus.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CSystem.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CSystem.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/IBaseWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/DebugAssert.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/EmptyCanvas.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Exception.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Logger.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/ExportImport.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/SystemIncludes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/WindowDesc.h"
)

ADD_LIBRARY(CoreLib SHARED
    "${CORE_SOURCE}"
    "${CORE_HEADRES}"
)

IF (UNIX)
    FIND_PACKAGE(X11 REQUIRED)

    TARGET_INCLUDE_DIRECTORIES(CoreLib PUBLIC ${X11_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(CoreLib PUBLIC ${X11_LIBRARIES})
endif()

TARGET_INCLUDE_DIRECTORIES(CoreLib PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_PRECOMPILE_HEADERS(CoreLib PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/Core.h")

TARGET_COMPILE_DEFINITIONS(CoreLib PRIVATE _BEAST_CORE_EXPORTS)

