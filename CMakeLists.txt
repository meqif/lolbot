cmake_minimum_required (VERSION 2.8)

# Find Ragel executable
FIND_PROGRAM(RAGEL_EXECUTABLE ragel)
IF(NOT RAGEL_EXECUTABLE)
  MESSAGE(SEND_ERROR "Ragel executable not found, you need it in order to build loldrop")
ENDIF(NOT RAGEL_EXECUTABLE)

set (source
xdcc.c
main.c
)

set(CMAKE_C_FLAGS "-Wall")
add_executable (loldrop ${source})
