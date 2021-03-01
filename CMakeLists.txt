cmake_minimum_required(VERSION 3.16.0)

project(PokeFinder)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(Externals/nlohmann)

enable_testing()

add_subdirectory(Source)
