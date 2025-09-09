SET(CORE_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/AppStatus.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/EmptyCanvas.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Exception.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/IWindow.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Logger.cpp"
)
SET(CORE_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AbLimits.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AppStatus.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/CSystem.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/EmptyCanvas.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Exception.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/IWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Logger.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/SystemIncludes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/WindowDesc.h"
)

ADD_LIBRARY(CoreLib 
    "${CORE_SOURCE}"
    "${CORE_HEADRES}"
)

IF (UNIX)
    FIND_PACKAGE(X11 REQUIRED)

    TARGET_INCLUDE_DIRECTORIES(CoreLib PRIVATE ${X11_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(CoreLib PRIVATE ${X11_LIBRARIES})
endif()

TARGET_INCLUDE_DIRECTORIES(CoreLib PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_SOURCES(CoreLib PUBLIC
    "${CORE_SOURCE}"
    "${CORE_HEADRES}"
)
