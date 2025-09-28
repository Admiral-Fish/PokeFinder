cmake_minimum_required(VERSION 3.31)

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

project(PokeFinder)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

option(TEST "Run Tests" OFF)

if (TEST)
    add_compile_definitions(TEST)
    enable_testing()
endif ()

add_subdirectory(Source)
