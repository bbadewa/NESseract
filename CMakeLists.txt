cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(NESseract C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE NESseractSource 
        "src/*.h"
        "src/*.c"
        "src/*.cpp"
)

add_executable(nesseract ${NESseractSource})



target_compile_options(nesseract PRIVATE -pthread -O0)
target_link_libraries(nesseract PRIVATE raylib pthread)


