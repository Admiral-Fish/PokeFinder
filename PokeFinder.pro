#-------------------------------------------------
#
# Project created by QtCreator 2017-07-28T23:02:16
#
#-------------------------------------------------

QT       += core gui
QT       += xml
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokeFinder
TEMPLATE = app
VERSION = 1.3.0
QMAKE_TARGET_DESCRIPTION = PokeFinder
QMAKE_TARGET_COPYRIGHT = Admiral_Fish

RC_ICONS += pokefinder.ico
ICON += pokefinder.icns

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += \
    Languages/PokeFinder_en.ts \
    Languages/PokeFinder_fr.ts \
    Languages/PokeFinder_es.ts \
    Languages/PokeFinder_de.ts \
    Languages/PokeFinder_it.ts \
    Languages/PokeFinder_ja.ts \
    Languages/PokeFinder_ko.ts \
    Languages/PokeFinder_zh_Hans_CN.ts

RESOURCES += \
    PokeFinderCore/encountertables.qrc \
    PokeFinderCore/text.qrc \
    resources.qrc

FORMS += \
    Forms/Controls/IVFilter.ui \
    Forms/Gen3/Eggs3.ui \
    Forms/Gen3/GameCubeRTC.ui \
    Forms/Gen3/IDs3.ui \
    Forms/Gen3/JirachiGeneration.ui \
    Forms/Gen3/PIDtoIVs.ui \
    Forms/Gen3/PokeSpot.ui \
    Forms/Gen3/ProfileManager3.ui \
    Forms/Gen3/ProfileManager3NewEdit.ui \
    Forms/Gen3/SeedToTime3.ui \
    Forms/Gen3/Stationary3.ui \
    Forms/Gen3/Wild3.ui \
    Forms/Gen4/ProfileManager4.ui \
    Forms/Gen4/ProfileManager4NewEdit.ui \
    Forms/Util/IVtoPID.ui \
    Forms/Util/Researcher.ui \
    Forms/MainWindow.ui

HEADERS += \
    Forms/Controls/IVFilter.hpp \
    Forms/Controls/QCheckList.hpp \
    Forms/Controls/QTextBox.hpp \
    Forms/Gen3/Eggs3.hpp \
    Forms/Gen3/GameCubeRTC.hpp \
    Forms/Gen3/IDs3.hpp \
    Forms/Gen3/JirachiGeneration.hpp \
    Forms/Gen3/PIDtoIVs.hpp \
    Forms/Gen3/PokeSpot.hpp \
    Forms/Gen3/ProfileManager3.hpp \
    Forms/Gen3/ProfileManager3NewEdit.hpp \
    Forms/Gen3/SeedToTime3.hpp \
    Forms/Gen3/Stationary3.hpp \
    Forms/Gen3/Wild3.hpp \
    Forms/Gen4/ProfileManager4.hpp \
    Forms/Gen4/ProfileManager4NewEdit.hpp \
    Forms/Util/IVtoPID.hpp \
    Forms/Util/Researcher.hpp \
    Forms/MainWindow.hpp \
    Models/Gen3/Egg3Model.hpp \
    Models/Gen3/PokeSpotModel.hpp \
    Models/Gen3/Profile3Model.hpp \
    Models/Gen3/Searcher3Model.hpp \
    Models/Gen3/Stationary3Model.hpp \
    Models/Gen3/Wild3Model.hpp \
    Models/Gen4/Profile4Model.hpp \
    Models/ResearcherModel.hpp \
    PokeFinderCore/Gen3/Egg3.hpp \
    PokeFinderCore/Gen3/EncounterArea3.hpp \
    PokeFinderCore/Gen3/Frame3.hpp \
    PokeFinderCore/Gen3/Generator3.hpp \
    PokeFinderCore/Gen3/NatureLock.hpp \
    PokeFinderCore/Gen3/Profile3.hpp \
    PokeFinderCore/Gen3/Searcher3.hpp \
    PokeFinderCore/Gen4/Profile4.hpp \
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
    Util/ResearcherFrame.hpp

SOURCES += \
    Forms/Controls/IVFilter.cpp \
    Forms/Controls/QCheckList.cpp \
    Forms/Controls/QTextBox.cpp \
    Forms/Gen3/Eggs3.cpp \
    Forms/Gen3/GameCubeRTC.cpp \
    Forms/Gen3/IDs3.cpp \
    Forms/Gen3/JirachiGeneration.cpp \
    Forms/Gen3/PIDtoIVs.cpp \
    Forms/Gen3/PokeSpot.cpp \
    Forms/Gen3/ProfileManager3.cpp \
    Forms/Gen3/ProfileManager3NewEdit.cpp \
    Forms/Gen3/SeedToTime3.cpp \
    Forms/Gen3/Stationary3.cpp \
    Forms/Gen3/Wild3.cpp \
    Forms/Gen4/help.cpp \
    Forms/Gen4/ProfileManager4.cpp \
    Forms/Gen4/ProfileManager4NewEdit.cpp \
    Forms/Util/IVtoPID.cpp \
    Forms/Util/Researcher.cpp \
    Forms/MainWindow.cpp \
    Models/Gen3/Egg3Model.cpp \
    Models/Gen3/PokeSpotModel.cpp \
    Models/Gen3/Profile3Model.cpp \
    Models/Gen3/Searcher3Model.cpp \
    Models/Gen3/Stationary3Model.cpp \
    Models/Gen3/Wild3Model.cpp \
    Models/Gen4/Profile4Model.cpp \
    Models/ResearcherModel.cpp \
    PokeFinderCore/Gen3/Egg3.cpp \
    PokeFinderCore/Gen3/EncounterArea3.cpp \
    PokeFinderCore/Gen3/Frame3.cpp \
    PokeFinderCore/Gen3/Generator3.cpp \
    PokeFinderCore/Gen3/NatureLock.cpp \
    PokeFinderCore/Gen3/Profile3.cpp \
    PokeFinderCore/Gen3/Searcher3.cpp \
    PokeFinderCore/Gen4/Profile4.cpp \
    PokeFinderCore/Objects/EncounterArea.cpp \
    PokeFinderCore/Objects/EncounterSlot.cpp \
    PokeFinderCore/Objects/Frame.cpp \
    PokeFinderCore/Objects/FrameCompare.cpp \
    PokeFinderCore/Objects/Nature.cpp \
    PokeFinderCore/Objects/Power.cpp \
    PokeFinderCore/Objects/Profile.cpp \
    PokeFinderCore/Objects/Utilities.cpp \
    PokeFinderCore/RNG/LCRNG.cpp \
    PokeFinderCore/RNG/LCRNG64.cpp \
    PokeFinderCore/RNG/MTRNG.cpp \
    PokeFinderCore/RNG/RNGCache.cpp \
    PokeFinderCore/RNG/RNGEuclidean.cpp \
    PokeFinderCore/RNG/SFMT.cpp \
    PokeFinderCore/RNG/TinyMT.cpp \
    PokeFinderCore/Translator.cpp \
    Util/ResearcherFrame.cpp \
    main.cpp
