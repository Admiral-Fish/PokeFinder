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

    if (NOT "$ENV{APPVEYOR_REPO_BRANCH}" STREQUAL "")
        set(GIT_BRANCH "$ENV{APPVEYOR_REPO_BRANCH}")
    endif ()

    add_definitions("-DGIT_BRANCH=${GIT_BRANCH}")
    add_definitions("-DGIT_COMMIT=${GIT_COMMIT}")

    configure_file(
        ${CMAKE_SOURCE_DIR}/Source/Form/version.h.in
        ${CMAKE_BINARY_DIR}/generated/version.h
    )
endif ()

include_directories(${CMAKE_BINARY_DIR}/generated)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core LinguistTools Network Widgets)

set(PROJECT_TS
    i18n/PokeFinder_fr.ts
    i18n/PokeFinder_es.ts
    i18n/PokeFinder_de.ts
    i18n/PokeFinder_it.ts
    i18n/PokeFinder_ja.ts
    i18n/PokeFinder_ko.ts
    i18n/PokeFinder_zh.ts
)

set(PROJECT_QRC
    resources.qrc
)

set(PROJECT_UI
    Controls/EggSettings.ui
    Controls/Filter.ui
    Controls/IDsFilter.ui
    Gen3/Eggs3.ui
    Gen3/IDs3.ui
    Gen3/GameCube.ui
    Gen3/Profile/ProfileEditor3.ui
    Gen3/Profile/ProfileManager3.ui
    Gen3/Static3.ui
    Gen3/Tools/GameCubeSeedFinder.ui
    Gen3/Tools/JirachiAdvancer.ui
    Gen3/Tools/PIDToIV.ui
    Gen3/Tools/PokeSpot.ui
    Gen3/Tools/SeedToTime3.ui
    Gen3/Tools/SpindaPainter.ui
    Gen3/Wild3.ui
    Gen4/Eggs4.ui
    Gen4/Event4.ui
    Gen4/IDs4.ui
    Gen4/Profile/ProfileEditor4.ui
    Gen4/Profile/ProfileManager4.ui
    Gen4/Static4.ui
    Gen4/Tools/ChainedSID.ui
    Gen4/Tools/RoamerMap.ui
    Gen4/Tools/SearchCalls.ui
    Gen4/Tools/SearchCoinFlips.ui
    Gen4/Tools/SeedToTime4.ui
    Gen4/Wild4.ui
    Gen5/DreamRadar.ui
    Gen5/HiddenGrotto.ui
    Gen5/Eggs5.ui
    Gen5/Event5.ui
    Gen5/IDs5.ui
    Gen5/Profile/ProfileCalibrator5.ui
    Gen5/Profile/ProfileEditor5.ui
    Gen5/Profile/ProfileManager5.ui
    Gen5/Static5.ui
    Gen5/Wild5.ui
    Gen8/Eggs8.ui
    Gen8/Event8.ui
    Gen8/IDs8.ui
    Gen8/Profile/ProfileEditor8.ui
    Gen8/Profile/ProfileManager8.ui
    Gen8/Raids.ui
    Gen8/Static8.ui
    Gen8/Tools/DenMap.ui
    Gen8/Underground.ui
    Gen8/Wild8.ui
    MainWindow.ui
    Util/EncounterLookup.ui
    Util/IVCalculator.ui
    Util/IVToPID.ui
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
    Controls/TableView.cpp
    Controls/TableView.hpp
    Controls/TextBox.cpp
    Controls/TextBox.hpp
    Gen3/Eggs3.cpp
    Gen3/Eggs3.hpp
    Gen3/IDs3.cpp
    Gen3/IDs3.hpp
    Gen3/GameCube.cpp
    Gen3/GameCube.hpp
    Gen3/Profile/ProfileEditor3.cpp
    Gen3/Profile/ProfileEditor3.hpp
    Gen3/Profile/ProfileManager3.cpp
    Gen3/Profile/ProfileManager3.hpp
    Gen3/Static3.cpp
    Gen3/Static3.hpp
    Gen3/Tools/GameCubeSeedFinder.cpp
    Gen3/Tools/GameCubeSeedFinder.hpp
    Gen3/Tools/JirachiAdvancer.cpp
    Gen3/Tools/JirachiAdvancer.hpp
    Gen3/Tools/PIDToIV.cpp
    Gen3/Tools/PIDToIV.hpp
    Gen3/Tools/PokeSpot.cpp
    Gen3/Tools/PokeSpot.hpp
    Gen3/Tools/SeedToTime3.cpp
    Gen3/Tools/SeedToTime3.hpp
    Gen3/Tools/SpindaPainter.cpp
    Gen3/Tools/SpindaPainter.hpp
    Gen3/Wild3.cpp
    Gen3/Wild3.hpp
    Gen4/Eggs4.cpp
    Gen4/Eggs4.hpp
    Gen4/Event4.cpp
    Gen4/Event4.hpp
    Gen4/IDs4.cpp
    Gen4/IDs4.hpp
    Gen4/Profile/ProfileEditor4.cpp
    Gen4/Profile/ProfileEditor4.hpp
    Gen4/Profile/ProfileManager4.cpp
    Gen4/Profile/ProfileManager4.hpp
    Gen4/Static4.cpp
    Gen4/Static4.hpp
    Gen4/Tools/ChainedSID.cpp
    Gen4/Tools/ChainedSID.hpp
    Gen4/Tools/RoamerMap.cpp
    Gen4/Tools/RoamerMap.hpp
    Gen4/Tools/SearchCalls.cpp
    Gen4/Tools/SearchCalls.hpp
    Gen4/Tools/SearchCoinFlips.cpp
    Gen4/Tools/SearchCoinFlips.hpp
    Gen4/Tools/SeedToTime4.cpp
    Gen4/Tools/SeedToTime4.hpp
    Gen4/Wild4.cpp
    Gen4/Wild4.hpp
    Gen5/DreamRadar.cpp
    Gen5/DreamRadar.hpp
    Gen5/Eggs5.cpp
    Gen5/Eggs5.hpp
    Gen5/Event5.cpp
    Gen5/Event5.hpp
    Gen5/HiddenGrotto.cpp
    Gen5/HiddenGrotto.hpp
    Gen5/IDs5.cpp
    Gen5/IDs5.hpp
    Gen5/Profile/ProfileCalibrator5.cpp
    Gen5/Profile/ProfileCalibrator5.hpp
    Gen5/Profile/ProfileEditor5.cpp
    Gen5/Profile/ProfileEditor5.hpp
    Gen5/Profile/ProfileManager5.cpp
    Gen5/Profile/ProfileManager5.hpp
    Gen5/Static5.cpp
    Gen5/Static5.hpp
    Gen5/Wild5.cpp
    Gen5/Wild5.hpp
    Gen8/Eggs8.cpp
    Gen8/Eggs8.hpp
    Gen8/Event8.cpp
    Gen8/Event8.hpp
    Gen8/IDs8.cpp
    Gen8/IDs8.hpp
    Gen8/Profile/ProfileEditor8.cpp
    Gen8/Profile/ProfileEditor8.hpp
    Gen8/Profile/ProfileManager8.cpp
    Gen8/Profile/ProfileManager8.hpp
    Gen8/Raids.cpp
    Gen8/Raids.hpp
    Gen8/Static8.cpp
    Gen8/Static8.hpp
    Gen8/Tools/DenMap.cpp
    Gen8/Tools/DenMap.hpp
    Gen8/Underground.cpp
    Gen8/Underground.hpp
    Gen8/Wild8.cpp
    Gen8/Wild8.hpp
    MainWindow.cpp
    MainWindow.hpp
    Util/EncounterLookup.cpp
    Util/EncounterLookup.hpp
    Util/IVCalculator.cpp
    Util/IVCalculator.hpp
    Util/IVToPID.cpp
    Util/IVToPID.hpp
    Util/Researcher.cpp
    Util/Researcher.hpp
    Util/Settings.cpp
    Util/Settings.hpp
)

qt_add_translation(PROJECT_QM ${PROJECT_TS} OPTIONS -nounfinished -removeidentical)
include(EmbedTranslations)
embed_translations(PROJECT_I18N ${PROJECT_QM})

add_library(PokeFinderForm STATIC
    ${PROJECT_I18N}
    ${PROJECT_QRC}
    ${PROJECT_UI}
    ${PROJECT_SOURCES}
)

target_link_libraries(PokeFinderForm PRIVATE PokeFinderCore PokeFinderModel Qt6::Core Qt6::Network Qt6::Widgets)
