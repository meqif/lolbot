cmake_minimum_required (VERSION 2.8)

# Find Ragel executable
FIND_PROGRAM(RAGEL_EXECUTABLE ragel)
IF(NOT RAGEL_EXECUTABLE)
  MESSAGE(SEND_ERROR "Ragel executable not found, you need it in order to build loldrop")
ENDIF(NOT RAGEL_EXECUTABLE)

ADD_CUSTOM_COMMAND (
    OUTPUT ${CMAKE_SOURCE_DIR}/src/xdcc.c
    COMMAND ragel -G2 -o ${CMAKE_SOURCE_DIR}/src/xdcc.c ${CMAKE_SOURCE_DIR}/src/xdcc.rl
    DEPENDS ${CMAKE_SOURCE_DIR}/src/xdcc.rl
)

FILE(GLOB source src/*.c)
LIST(APPEND source src/xdcc.c)

set(CMAKE_C_FLAGS "-Wall")
add_executable (loldrop ${source} src/main.c)

#set (tests
#	tests/xdcc_tests
#)
#include_directories (${CMAKE_SOURCE_DIR}/src)
#foreach (test ${tests})
#	add_executable(${test} ${test}.c ${source})
#endforeach(test)
#
#add_custom_target (
#	test
#    COMMAND sh tests/runtests.sh
# )
