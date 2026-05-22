cmake_minimum_required(VERSION 3.11)
project(Arthur)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/master.tar.gz
)
FetchContent_MakeAvailable(raylib)

add_executable(Arthur src/main.cpp)
target_include_directories(Arthur PRIVATE src)
target_link_libraries(Arthur raylib)