#-------------------------------------------------
#
# Project created by QtCreator 2017-07-28T23:02:16
#
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION, 5): error("You need at least Qt 5.11 to build PokeFinder")
equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 11): error("You need at least Qt 5.11 to build firebird")
!exists("PokeFinderCore/resources_core.qrc"): error("You are missing the submodule required to build PokeFinder")

QT += network widgets
CONFIG += c++14

TARGET = PokeFinder
TEMPLATE = app
VERSION = 2.1.0
QMAKE_TARGET_DESCRIPTION = PokeFinder
QMAKE_TARGET_COPYRIGHT = Admiral_Fish

RC_ICONS += pokefinder.ico
ICON += pokefinder.icns

DEFINES += QT_DEPRECATED_WARNINGS

TRANSLATIONS += \
    Translations/PokeFinder_en.ts \
    Translations/PokeFinder_fr.ts \
    Translations/PokeFinder_es.ts \
    Translations/PokeFinder_de.ts \
    Translations/PokeFinder_it.ts \
    Translations/PokeFinder_ja.ts \
    Translations/PokeFinder_ko.ts \
    Translations/PokeFinder_zh_Hans_CN.ts

RESOURCES += \
    PokeFinderCore/resources_core.qrc \
    resources.qrc

FORMS += \
    Forms/Controls/IVFilter.ui \
    Forms/Gen3/Eggs3.ui \
    Forms/Gen3/GameCubeRTC.ui \
    Forms/Gen3/IDs3.ui \
    Forms/Gen3/JirachiPattern.ui \
    Forms/Gen3/PIDtoIVs.ui \
    Forms/Gen3/PokeSpot.ui \
    Forms/Gen3/ProfileManager3.ui \
    Forms/Gen3/ProfileManager3NewEdit.ui \
    Forms/Gen3/SeedToTime3.ui \
    Forms/Gen3/Stationary3.ui \
    Forms/Gen3/Wild3.ui \
    Forms/Gen4/Eggs4.ui \
    Forms/Gen4/IDs4.ui \
    Forms/Gen4/ProfileManager4.ui \
    Forms/Gen4/ProfileManager4NewEdit.ui \
    Forms/Gen4/RoamerMap.ui \
    Forms/Gen4/SearchCalls.ui \
    Forms/Gen4/SearchCoinFlips.ui \
    Forms/Gen4/SeedtoTime4.ui \
    Forms/Gen4/Stationary4.ui \
    Forms/Gen4/Wild4.ui \
    Forms/Util/IVtoPID.ui \
    Forms/Util/Researcher.ui \
    Forms/MainWindow.ui

HEADERS += \
    Forms/Controls/IVFilter.hpp \
    Forms/Controls/Label.hpp \
    Forms/Controls/QCheckList.hpp \
    Forms/Controls/QTextBox.hpp \
    Forms/Controls/TableView.hpp \
    Forms/Gen3/Eggs3.hpp \
    Forms/Gen3/GameCubeRTC.hpp \
    Forms/Gen3/IDs3.hpp \
    Forms/Gen3/JirachiPattern.hpp \
    Forms/Gen3/PIDtoIVs.hpp \
    Forms/Gen3/PokeSpot.hpp \
    Forms/Gen3/ProfileManager3.hpp \
    Forms/Gen3/ProfileManager3NewEdit.hpp \
    Forms/Gen3/SeedToTime3.hpp \
    Forms/Gen3/Stationary3.hpp \
    Forms/Gen3/Wild3.hpp \
    Forms/Gen4/Eggs4.hpp \
    Forms/Gen4/IDs4.hpp \
    Forms/Gen4/ProfileManager4.hpp \
    Forms/Gen4/ProfileManager4NewEdit.hpp \
    Forms/Gen4/RoamerMap.hpp \
    Forms/Gen4/SearchCalls.hpp \
    Forms/Gen4/SearchCoinFlips.hpp \
    Forms/Gen4/SeedtoTime4.hpp \
    Forms/Gen4/Stationary4.hpp \
    Forms/Gen4/Wild4.hpp \
    Forms/Util/IVtoPID.hpp \
    Forms/Util/Researcher.hpp \
    Forms/MainWindow.hpp \
    Models/Gen3/Egg3Model.hpp \
    Models/Gen3/PokeSpotModel.hpp \
    Models/Gen3/Profile3Model.hpp \
    Models/Gen3/Searcher3Model.hpp \
    Models/Gen3/Stationary3Model.hpp \
    Models/Gen3/Wild3Model.hpp \
    Models/Gen4/Egg4Model.hpp \
    Models/Gen4/Profile4Model.hpp \
    Models/Gen4/Searcher4Model.hpp \
    Models/Gen4/SeedtoTime4Model.hpp \
    Models/Gen4/Stationary4Model.hpp \
    Models/Gen4/Wild4Model.hpp \
    Models/ResearcherModel.hpp \
    PokeFinderCore/Gen3/Egg3.hpp \
    PokeFinderCore/Gen3/EncounterArea3.hpp \
    PokeFinderCore/Gen3/Encounters3.hpp \
    PokeFinderCore/Gen3/Frame3.hpp \
    PokeFinderCore/Gen3/Generator3.hpp \
    PokeFinderCore/Gen3/NatureLock.hpp \
    PokeFinderCore/Gen3/Profile3.hpp \
    PokeFinderCore/Gen3/Searcher3.hpp \
    PokeFinderCore/Gen4/Egg4.hpp \
    PokeFinderCore/Gen4/EncounterArea4.hpp \
    PokeFinderCore/Gen4/Encounters4.hpp \
    PokeFinderCore/Gen4/Frame4.hpp \
    PokeFinderCore/Gen4/Generator4.hpp \
    PokeFinderCore/Gen4/HGSSRoamer.hpp \
    PokeFinderCore/Gen4/Profile4.hpp \
    PokeFinderCore/Gen4/Searcher4.hpp \
    PokeFinderCore/Objects/Egg.hpp \
    PokeFinderCore/Objects/Encounter.hpp \
    PokeFinderCore/Objects/EncounterArea.hpp \
    PokeFinderCore/Objects/EncounterSlot.hpp \
    PokeFinderCore/Objects/Frame.hpp \
    PokeFinderCore/Objects/FrameCompare.hpp \
    PokeFinderCore/Objects/Game.hpp \
    PokeFinderCore/Objects/Generator.hpp \
    PokeFinderCore/Objects/Lead.hpp \
    PokeFinderCore/Objects/Method.hpp \
    PokeFinderCore/Objects/Nature.hpp \
    PokeFinderCore/Objects/Power.hpp \
    PokeFinderCore/Objects/Profile.hpp \
    PokeFinderCore/Objects/Searcher.hpp \
    PokeFinderCore/Objects/Utilities.hpp \
    PokeFinderCore/RNG/IRNG.hpp \
    PokeFinderCore/RNG/IRNG64.hpp \
    PokeFinderCore/RNG/LCRNG.hpp \
    PokeFinderCore/RNG/LCRNG64.hpp \
    PokeFinderCore/RNG/MTRNG.hpp \
    PokeFinderCore/RNG/RNGCache.hpp \
    PokeFinderCore/RNG/RNGEuclidean.hpp \
    PokeFinderCore/RNG/SFMT.hpp \
    PokeFinderCore/RNG/TinyMT.hpp \
    PokeFinderCore/Translator.hpp \
    Util/DateTime.hpp \
    Util/ResearcherFrame.hpp

SOURCES += \
    Forms/Controls/IVFilter.cpp \
    Forms/Controls/Label.cpp \
    Forms/Controls/QCheckList.cpp \
    Forms/Controls/QTextBox.cpp \
    Forms/Controls/TableView.cpp \
    Forms/Gen3/Eggs3.cpp \
    Forms/Gen3/GameCubeRTC.cpp \
    Forms/Gen3/IDs3.cpp \
    Forms/Gen3/JirachiPattern.cpp \
    Forms/Gen3/PIDtoIVs.cpp \
    Forms/Gen3/PokeSpot.cpp \
    Forms/Gen3/ProfileManager3.cpp \
    Forms/Gen3/ProfileManager3NewEdit.cpp \
    Forms/Gen3/SeedToTime3.cpp \
    Forms/Gen3/Stationary3.cpp \
    Forms/Gen3/Wild3.cpp \
    Forms/Gen4/Eggs4.cpp \
    Forms/Gen4/IDs4.cpp \
    Forms/Gen4/ProfileManager4.cpp \
    Forms/Gen4/ProfileManager4NewEdit.cpp \
    Forms/Gen4/RoamerMap.cpp \
    Forms/Gen4/SearchCalls.cpp \
    Forms/Gen4/SearchCoinFlips.cpp \
    Forms/Gen4/SeedtoTime4.cpp \
    Forms/Gen4/Stationary4.cpp \
    Forms/Gen4/Wild4.cpp \
    Forms/Util/IVtoPID.cpp \
    Forms/Util/Researcher.cpp \
    Forms/MainWindow.cpp \
    Models/Gen3/Egg3Model.cpp \
    Models/Gen3/PokeSpotModel.cpp \
    Models/Gen3/Profile3Model.cpp \
    Models/Gen3/Searcher3Model.cpp \
    Models/Gen3/Stationary3Model.cpp \
    Models/Gen3/Wild3Model.cpp \
    Models/Gen4/Egg4Model.cpp \
    Models/Gen4/Profile4Model.cpp \
    Models/Gen4/Searcher4Model.cpp \
    Models/Gen4/SeedtoTime4Model.cpp \
    Models/Gen4/Stationary4Model.cpp \
    Models/Gen4/Wild4Model.cpp \
    Models/ResearcherModel.cpp \
    PokeFinderCore/Gen3/Egg3.cpp \
    PokeFinderCore/Gen3/EncounterArea3.cpp \
    PokeFinderCore/Gen3/Encounters3.cpp \
    PokeFinderCore/Gen3/Frame3.cpp \
    PokeFinderCore/Gen3/Generator3.cpp \
    PokeFinderCore/Gen3/NatureLock.cpp \
    PokeFinderCore/Gen3/Profile3.cpp \
    PokeFinderCore/Gen3/Searcher3.cpp \
    PokeFinderCore/Gen4/Egg4.cpp \
    PokeFinderCore/Gen4/EncounterArea4.cpp \
    PokeFinderCore/Gen4/Encounters4.cpp \
    PokeFinderCore/Gen4/Frame4.cpp \
    PokeFinderCore/Gen4/Generator4.cpp \
    PokeFinderCore/Gen4/HGSSRoamer.cpp \
    PokeFinderCore/Gen4/Profile4.cpp \
    PokeFinderCore/Gen4/Searcher4.cpp \
    PokeFinderCore/Objects/Egg.cpp \
    PokeFinderCore/Objects/EncounterArea.cpp \
    PokeFinderCore/Objects/EncounterSlot.cpp \
    PokeFinderCore/Objects/Frame.cpp \
    PokeFinderCore/Objects/FrameCompare.cpp \
    PokeFinderCore/Objects/Generator.cpp \
    PokeFinderCore/Objects/Nature.cpp \
    PokeFinderCore/Objects/Power.cpp \
    PokeFinderCore/Objects/Profile.cpp \
    PokeFinderCore/Objects/Searcher.cpp \
    PokeFinderCore/Objects/Utilities.cpp \
    PokeFinderCore/RNG/LCRNG.cpp \
    PokeFinderCore/RNG/LCRNG64.cpp \
    PokeFinderCore/RNG/MTRNG.cpp \
    PokeFinderCore/RNG/RNGCache.cpp \
    PokeFinderCore/RNG/RNGEuclidean.cpp \
    PokeFinderCore/RNG/SFMT.cpp \
    PokeFinderCore/RNG/TinyMT.cpp \
    PokeFinderCore/Translator.cpp \
    Util/DateTime.cpp \
    Util/ResearcherFrame.cpp \
    main.cpp
