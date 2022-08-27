project(PokeFinderForm)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/generated/version.h)
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND git rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    add_definitions("-DGIT_BRANCH=${GIT_BRANCH}")
    add_definitions("-DGIT_COMMIT=${GIT_COMMIT}")

    configure_file(
        ${CMAKE_SOURCE_DIR}/Source/Form/version.h.in
        ${CMAKE_BINARY_DIR}/generated/version.h
    )
endif ()

include_directories(${CMAKE_BINARY_DIR}/generated)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

set(PROJECT_TS
    i18n/PokeFinder_fr.ts
    i18n/PokeFinder_es.ts
    i18n/PokeFinder_de.ts
    i18n/PokeFinder_it.ts
    i18n/PokeFinder_ja.ts
    i18n/PokeFinder_ko.ts
    i18n/PokeFinder_zh.ts
)

set(PROJECT_UI
    Controls/EggSettings.ui
    Controls/Filter.ui
    Controls/IDsFilter.ui
    Controls/IVFilter.ui
    Gen3/IDs3.ui
    Gen3/Profile/ProfileEditor3.ui
    Gen3/Profile/ProfileManager3.ui
    Gen3/Static3.ui
    Gen3/Wild3.ui
    MainWindow.ui
    Util/IVCalculator.ui
    Util/Researcher.ui
    Util/Settings.ui
)

set(PROJECT_SOURCES
    Controls/CheckList.cpp
    Controls/CheckList.hpp
    Controls/ComboBox.cpp
    Controls/ComboBox.hpp
    Controls/ComboMenu.cpp
    Controls/ComboMenu.hpp
    Controls/Controls.hpp
    Controls/DateEdit.cpp
    Controls/DateEdit.hpp
    Controls/DateTimeEdit.cpp
    Controls/DateTimeEdit.hpp
    Controls/EggSettings.cpp
    Controls/EggSettings.hpp
    Controls/Filter.cpp
    Controls/Filter.hpp
    Controls/GraphicsPixmapItem.cpp
    Controls/GraphicsPixmapItem.hpp
    Controls/IDsFilter.cpp
    Controls/IDsFilter.hpp
    Controls/IVFilter.cpp
    Controls/IVFilter.hpp
    Controls/Label.cpp
    Controls/Label.hpp
    Controls/TableView.cpp
    Controls/TableView.hpp
    Controls/TextBox.cpp
    Controls/TextBox.hpp
    Gen3/IDs3.cpp
    Gen3/IDs3.hpp
    Gen3/Profile/ProfileEditor3.cpp
    Gen3/Profile/ProfileEditor3.hpp
    Gen3/Profile/ProfileManager3.cpp
    Gen3/Profile/ProfileManager3.hpp
    Gen3/Static3.cpp
    Gen3/Static3.hpp
    Gen3/Wild3.cpp
    Gen3/Wild3.hpp
    MainWindow.cpp
    MainWindow.hpp
    Util/IVCalculator.cpp
    Util/IVCalculator.hpp
    Util/Researcher.cpp
    Util/Researcher.hpp
    Util/Settings.cpp
    Util/Settings.hpp
)

add_library(PokeFinderForm STATIC
    ${PROJECT_I18N}
    ${PROJECT_UI}
    ${PROJECT_SOURCES}
)

target_link_libraries(PokeFinderForm PRIVATE PokeFinderCore PokeFinderModel Qt6::Core Qt6::Network Qt6::Widgets)
