MESSAGE(STATUS "Now we get into the wrap dir to compiile the lib of wrap.a " ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE(${PROJECT_SOURCE_DIR}/src/variables.txt)
INCLUDE_DIRECTORIES(${INC_DIR})

SET(LIB_SRC wrap.c)
#SET(LIBRARY_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH})

MESSAGE(STATUS "The wrap.a is compiled to dir :" ${LIBRARY_OUTPUT_PATH})

#INCLUDE_DIRECTORIES(${INC_DIR})
ADD_LIBRARY(wrap ${LIB_SRC})
