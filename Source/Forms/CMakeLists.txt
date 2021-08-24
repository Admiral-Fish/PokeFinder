project(PokeFinder VERSION 3.0.1 LANGUAGES CXX)

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
    ${CMAKE_SOURCE_DIR}/Source/Forms/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
)
include_directories(${CMAKE_BINARY_DIR}/generated)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core LinguistTools Network Widgets REQUIRED)

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
    "${CMAKE_SOURCE_DIR}/Externals/QDarkStyleSheet/qdarkstyle/dark/style.qrc"
    resources.qrc
)

set(PROJECT_UI	
    Controls/EggSettings.ui
    Controls/Filter.ui
    Controls/IVFilter.ui
    Gen3/Eggs3.ui
    Gen3/GameCube.ui
    Gen3/IDs3.ui
    Gen3/Profile/ProfileEditor3.ui
    Gen3/Profile/ProfileManager3.ui
    Gen3/Stationary3.ui
    Gen3/Tools/GameCubeRTC.ui
    Gen3/Tools/GameCubeSeedFinder.ui
    Gen3/Tools/JirachiPattern.ui
    Gen3/Tools/PIDIV.ui
    Gen3/Tools/PokeSpot.ui
    Gen3/Tools/SeedTime3.ui
    Gen3/Tools/SpindaPainter.ui
    Gen3/Wild3.ui
    Gen4/Eggs4.ui
    Gen4/IDs4.ui
    Gen4/Profile/ProfileEditor4.ui
    Gen4/Profile/ProfileManager4.ui
    Gen4/Stationary4.ui
    Gen4/Tools/ChainedSID.ui
    Gen4/Tools/Poketch.ui
    Gen4/Tools/RoamerMap.ui
    Gen4/Tools/SearchCalls.ui
    Gen4/Tools/SearchCoinFlips.ui
    Gen4/Tools/SeedtoTime4.ui
    Gen4/Wild4.ui
    Gen5/DreamRadar.ui
    Gen5/Eggs5.ui
    Gen5/Event5.ui
    Gen5/HiddenGrotto.ui
    Gen5/IDs5.ui
    Gen5/Profile/ProfileCalibrator5.ui
    Gen5/Profile/ProfileEditor5.ui
    Gen5/Profile/ProfileManager5.ui
    Gen5/Stationary5.ui
    MainWindow.ui
    Util/EncounterLookup.ui
    Util/IVCalculator.ui
    Util/IVtoPID.ui
    Util/Researcher.ui
    Util/Settings.ui
)

set(PROJECT_SOURCES
    Controls/CheckList.cpp
    Controls/ComboBox.cpp
    Controls/DateEdit.cpp
    Controls/DateTimeEdit.cpp
    Controls/EggSettings.cpp
    Controls/Filter.cpp
    Controls/GraphicsPixmapItem.cpp
    Controls/IVFilter.cpp
    Controls/Label.cpp
    Controls/TableView.cpp
    Controls/TextBox.cpp
    Gen3/Eggs3.cpp
    Gen3/GameCube.cpp
    Gen3/IDs3.cpp
    Gen3/Profile/ProfileEditor3.cpp
    Gen3/Profile/ProfileManager3.cpp
    Gen3/Stationary3.cpp
    Gen3/Tools/GameCubeRTC.cpp
    Gen3/Tools/GameCubeSeedFinder.cpp
    Gen3/Tools/JirachiPattern.cpp
    Gen3/Tools/PIDIV.cpp
    Gen3/Tools/PokeSpot.cpp
    Gen3/Tools/SeedTime3.cpp
    Gen3/Tools/SpindaPainter.cpp
    Gen3/Wild3.cpp
    Gen4/Eggs4.cpp
    Gen4/IDs4.cpp
    Gen4/Profile/ProfileEditor4.cpp
    Gen4/Profile/ProfileManager4.cpp
    Gen4/Stationary4.cpp
    Gen4/Tools/ChainedSID.cpp
    Gen4/Tools/Poketch.cpp
    Gen4/Tools/RoamerMap.cpp
    Gen4/Tools/SearchCalls.cpp
    Gen4/Tools/SearchCoinFlips.cpp
    Gen4/Tools/SeedtoTime4.cpp
    Gen4/Wild4.cpp
    Gen5/DreamRadar.cpp
    Gen5/Eggs5.cpp
    Gen5/Event5.cpp
    Gen5/HiddenGrotto.cpp
    Gen5/IDs5.cpp
    Gen5/Profile/ProfileCalibrator5.cpp
    Gen5/Profile/ProfileEditor5.cpp
    Gen5/Profile/ProfileManager5.cpp
    Gen5/Stationary5.cpp
    MainWindow.cpp
    Models/Gen3/EggModel3.cpp
    Models/Gen3/GameCubeModel.cpp
    Models/Gen3/GameCubeRTCModel.cpp
    Models/Gen3/IDModel3.cpp
    Models/Gen3/PIDIVModel.cpp
    Models/Gen3/PokeSpotModel.cpp
    Models/Gen3/ProfileModel3.cpp
    Models/Gen3/SeedTimeModel3.cpp
    Models/Gen3/StationaryModel3.cpp
    Models/Gen3/WildModel3.cpp
    Models/Gen4/EggModel4.cpp
    Models/Gen4/IDModel4.cpp
    Models/Gen4/ProfileModel4.cpp
    Models/Gen4/SeedtoTimeModel4.cpp
    Models/Gen4/StationaryModel4.cpp
    Models/Gen4/WildModel4.cpp
    Models/Gen5/DreamRadarModel.cpp
    Models/Gen5/EggModel5.cpp
    Models/Gen5/EventModel5.cpp
    Models/Gen5/HiddenGrottoModel.cpp
    Models/Gen5/IDModel5.cpp
    Models/Gen5/ProfileModel5.cpp
    Models/Gen5/ProfileSearcherModel5.cpp
    Models/Gen5/StationaryModel5.cpp
    Models/Util/ResearcherModel.cpp
    Util/EncounterLookup.cpp
    Util/IVCalculator.cpp
    Util/IVtoPID.cpp
    Util/Researcher.cpp
    Util/Settings.cpp
    main.cpp
)

qt5_add_translation(PROJECT_QM ${PROJECT_TS} OPTIONS -nounfinished -removeidentical)
include(EmbedTranslations)
embed_translations(PROJECT_I18N ${PROJECT_QM})

if (WIN32)
    add_executable(PokeFinder
        WIN32
        ${PROJECT_I18N}
        ${PROJECT_QRC}
        ${PROJECT_UI}
        ${PROJECT_SOURCES}
        appicon.rc
    )
elseif (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE pokefinder.icns)
    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/Images/pokefinder.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(PokeFinder
        MACOSX_BUNDLE
        ${PROJECT_I18N}
        ${PROJECT_QRC}
        ${PROJECT_UI}
        ${PROJECT_SOURCES}
        ${APP_ICON_MACOSX}
    )
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    add_executable(PokeFinder
        ${PROJECT_I18N}
        ${PROJECT_QRC}
        ${PROJECT_UI}
        ${PROJECT_SOURCES}
    )
endif ()

target_link_libraries(PokeFinder PRIVATE PokeFinderCore Qt5::Core Qt5::Network Qt5::Widgets)
