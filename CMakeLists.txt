cmake_minimum_required(VERSION 2.6)

project(3dengine CXX)

set(CXX_FLAGS
    -g
    -Wall
    -Werror
    )
 
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}") 
set(CMAKE_CXX_COMPILER "g++") 
set(CMAKE_CXX_FLAGS_DEBUG "-O0") 
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(engine)
add_subdirectory(demo)
