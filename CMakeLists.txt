cmake_minimum_required(VERSION 3.31)

if (APPLE AND NOT DEFINED CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

project(PokeFinder VERSION 4.3.2)

option(TEST "Run Tests" OFF)
option(SIMD "Enable SIMD" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package(Python3 3.14 COMPONENTS Interpreter REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Threads REQUIRED)

qt_standard_project_setup()

add_library(PokeFinderIncludes INTERFACE)
target_include_directories(PokeFinderIncludes INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Core)
if (TEST)
    add_compile_definitions(TEST)
    enable_testing()
    add_subdirectory(Test)
endif ()
add_subdirectory(Model)
add_subdirectory(Form)

qt_add_executable(PokeFinder WIN32 MACOSX_BUNDLE MANUAL_FINALIZATION main.cpp)

target_sources(PokeFinder PRIVATE
    "$<$<PLATFORM_ID:Windows>:appicon.rc>"
    "$<$<PLATFORM_ID:Darwin>:${CMAKE_CURRENT_SOURCE_DIR}/pokefinder.icns>"
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/pokefinder.icns PROPERTIES 
    MACOSX_PACKAGE_LOCATION "Resources"
)

set_target_properties(PokeFinder PROPERTIES 
    MACOSX_BUNDLE_ICON_FILE pokefinder.icns
)

target_link_libraries(PokeFinder PRIVATE PokeFinderCore PokeFinderForm Qt6::Core Qt6::Widgets Threads::Threads)

qt_finalize_target(PokeFinder)
