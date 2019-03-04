message(STATUS "Now we get into the demo dir to compile the bin  " ${CMAKE_CURRENT_SOURCE_DIR})

include(${PROJECT_SOURCE_DIR}/src/env.txt)

set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})
set(LINK_LIBS libcommon.a)
set(SRC_LIST demo_main.c)

include_directories(${INC_DIR})
link_directories(${LIB_DIR})

message(STATUS "We link lib:  " ${LINK_LIBS} "from the" ${LIB_DIR} )
add_executable(demo ${SRC_LIST})
target_link_libraries(demo ${LINK_LIBS})

