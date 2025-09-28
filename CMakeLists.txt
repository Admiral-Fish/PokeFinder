cmake_minimum_required(VERSION 3.31)

if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

project(PokeFinder VERSION 4.2.1)

option(TEST "Run Tests" OFF)
option(SIMD "Enable SIMD" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(.)

add_subdirectory(Core)
if (TEST)
    add_compile_definitions(TEST)
    enable_testing()
    add_subdirectory(Test)
endif ()
add_subdirectory(Model)
add_subdirectory(Form)

if (WIN32)
    add_executable(PokeFinder
        WIN32
        main.cpp
        appicon.rc
    )
elseif (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE pokefinder.icns)
    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/pokefinder.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(PokeFinder
        MACOSX_BUNDLE
        main.cpp
        ${APP_ICON_MACOSX}
    )
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    add_executable(PokeFinder
        main.cpp
    )
endif ()

target_link_libraries(PokeFinder PRIVATE PokeFinderCore PokeFinderForm Qt6::Core Qt6::Widgets)
