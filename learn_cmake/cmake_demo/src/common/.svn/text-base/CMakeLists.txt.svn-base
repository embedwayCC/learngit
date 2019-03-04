message(STATUS "Now we get into the common dir to compiile the lib of common.a " ${CMAKE_CURRENT_SOURCE_DIR})

include(${PROJECT_SOURCE_DIR}/src/env.txt)
include_directories(${INC_DIR})

set(LIB_SRC common.c)
set(LIBRARY_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH})

message(STATUS "The common.a is compiled to dir :" ${LIBRARY_OUTPUT_PATH})

include_directories(${INC_DIR})
add_library(common ${LIB_SRC})

