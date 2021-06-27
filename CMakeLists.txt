cmake_minimum_required(VERSION 3.16.0)

project(PokeFinder)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if (TEST)
    enable_testing()
endif ()

add_subdirectory(Source)
