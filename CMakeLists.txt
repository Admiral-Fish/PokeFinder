cmake_minimum_required(VERSION 3.16)

project(PokeFinder)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

option(TEST "Run Tests" OFF)
option(CORE_ONLY "Only Build Core Library" OFF)

if (TEST)
    add_compile_definitions(TEST)
    enable_testing()
endif ()

add_subdirectory(Source)
