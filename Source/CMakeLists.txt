project(PokeFinder VERSION 4.2.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_AUTORCC ON)

include_directories(.)

add_subdirectory(Core)
if (TEST)
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

target_link_libraries(PokeFinder PRIVATE PokeFinderForm Qt6::Core Qt6::Widgets)
