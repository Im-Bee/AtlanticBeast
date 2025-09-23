SET(APPLICATION_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/AppStatus.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/AppResources.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowDesc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/BaseWindowDetails.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Linux/BasicLinuxPolicy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Window/WindowPolicy/Win32/BasicWin32Policy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/UserInput.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/Input/KeysMap.cpp"
)
SET(APPLICATION_HEADRES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AppStatus.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/AppResources.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/IWindowPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/BasicSystemPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Linux/BasicLinuxPolicy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowPolicy/Win32/BasicWin32Policy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/BaseWindowDetails.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/IBaseWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowDesc.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Window/WindowEvents.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/UserInput.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/KeyBind.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/InputEvents.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/KeyList.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/KeysMap.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/Input/ControllerObject.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/EmptyWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/EmptyCanvas.hpp"
)

ADD_LIBRARY(AppLib SHARED
    "${APPLICATION_SOURCE}"
    "${APPLICATION_HEADRES}"
)

IF (UNIX)

    FIND_PACKAGE(X11 REQUIRED)
    FIND_LIBRARY(XI_LIB Xi REQUIRED)

    TARGET_INCLUDE_DIRECTORIES(AppLib PUBLIC ${X11_INCLUDE_DIR})
    TARGET_INCLUDE_DIRECTORIES(AppLib PUBLIC ${XI_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(AppLib PUBLIC ${X11_LIBRARIES})
    TARGET_LINK_LIBRARIES(AppLib PUBLIC ${XI_LIB})

endif()

TARGET_INCLUDE_DIRECTORIES(AppLib PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/"
)

TARGET_LINK_LIBRARIES(AppLib PUBLIC 
    CoreLib
)

TARGET_PRECOMPILE_HEADERS(AppLib PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Public/App.h")

TARGET_COMPILE_DEFINITIONS(AppLib PRIVATE _BEAST_CORE_EXPORTS)


