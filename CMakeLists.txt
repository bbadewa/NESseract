cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(NESseract C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE Emulator 
        "src/interpreter/*.h"
        "src/interpreter/*.c"
)

file(GLOB_RECURSE MachineSpy 
        "src/observer/MachineSpy.c"
        "src/compositor/lib/*.h"
)

# MachineSpy project options
add_executable(MachineSpy ${Emulator} ${MachineSpy})
target_compile_options(MachineSpy PRIVATE -pthread -O0)
target_link_libraries(MachineSpy PRIVATE raylib pthread)

#Testing suite project options



