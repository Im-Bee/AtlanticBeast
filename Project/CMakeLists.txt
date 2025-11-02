FILE(GLOB_RECURSE PRJT_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.hpp"
)

ADD_EXECUTABLE(AtlBee  
    "${PRJT_SOURCE}"
)

TARGET_LINK_LIBRARIES(AtlBee PRIVATE 
    CoreLib
    AppLib
    RenderingLib
)
