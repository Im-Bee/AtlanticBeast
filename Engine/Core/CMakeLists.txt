SET(CORE_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/AppStatus.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Exception.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Debug/Logger.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowDesc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/BaseWindowDetails.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/BaseWindowDetails.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/UserInput.cpp"
)
SET(CORE_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AbLimits.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AppStatus.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CSystem.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CSystem.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Exception.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/ExportImport.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/SystemIncludes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Debug/Assert.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Debug/Logger.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/IBaseWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowDesc.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowEvents.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/UserInput.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/InputEvents.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/EmptyCanvas.hpp"
)

ADD_LIBRARY(CoreLib SHARED
    "${CORE_SOURCE}"
    "${CORE_HEADRES}"
)

IF (UNIX)

    FIND_PACKAGE(X11 REQUIRED)
    FIND_LIBRARY(XI_LIB Xi REQUIRED)

    TARGET_INCLUDE_DIRECTORIES(CoreLib PUBLIC ${X11_INCLUDE_DIR})
    TARGET_INCLUDE_DIRECTORIES(CoreLib PUBLIC ${XI_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(CoreLib PUBLIC ${X11_LIBRARIES})
    TARGET_LINK_LIBRARIES(CoreLib PUBLIC ${XI_LIB})

endif()

TARGET_INCLUDE_DIRECTORIES(CoreLib PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_PRECOMPILE_HEADERS(CoreLib PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/Core.h")

TARGET_COMPILE_DEFINITIONS(CoreLib PRIVATE _BEAST_CORE_EXPORTS)

