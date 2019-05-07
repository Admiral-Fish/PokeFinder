lessThan(QT_MAJOR_VERSION, 5):error("You need at least Qt 5.11 to build PokeFinder")
equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 11):error("You need at least Qt 5.11 to build PokeFinder")

QT += concurrent network widgets
CONFIG += c++1z
CONFIG += lrelease embed_translations

TARGET = PokeFinder
TEMPLATE = app
VERSION = 2.4.0
QMAKE_TARGET_DESCRIPTION = PokeFinder
QMAKE_TARGET_COPYRIGHT = Admiral_Fish

RC_ICONS += Images/pokefinder.ico
ICON += Images/pokefinder.icns

DEFINES += QT_DEPRECATED_WARNINGS

TRANSLATIONS += \
    i18n/PokeFinder_en.ts \
    i18n/PokeFinder_fr.ts \
    i18n/PokeFinder_es.ts \
    i18n/PokeFinder_de.ts \
    i18n/PokeFinder_it.ts \
    i18n/PokeFinder_ja.ts \
    i18n/PokeFinder_ko.ts \
    i18n/PokeFinder_zh_Hans_CN.ts

RESOURCES += \
    QDarkStyleSheet/qdarkstyle/style.qrc \
    resources.qrc

FORMS += \
    Forms/Controls/EggSettings.ui \
    Forms/Controls/IVFilter.ui \
    Forms/Gen3/Eggs3.ui \
    Forms/Gen3/GameCube.ui \
    Forms/Gen3/GameCubeRTC.ui \
    Forms/Gen3/GameCubeSeedFinder.ui \
    Forms/Gen3/IDs3.ui \
    Forms/Gen3/JirachiPattern.ui \
    Forms/Gen3/PIDtoIVs.ui \
    Forms/Gen3/PokeSpot.ui \
    Forms/Gen3/ProfileEditor3.ui \
    Forms/Gen3/ProfileManager3.ui \
    Forms/Gen3/SeedToTime3.ui \
    Forms/Gen3/SpindaPainter.ui \
    Forms/Gen3/Stationary3.ui \
    Forms/Gen3/Wild3.ui \
    Forms/Gen4/ChainedSID.ui \
    Forms/Gen4/Eggs4.ui \
    Forms/Gen4/IDs4.ui \
    Forms/Gen4/ProfileEditor4.ui \
    Forms/Gen4/ProfileManager4.ui \
    Forms/Gen4/RoamerMap.ui \
    Forms/Gen4/SearchCalls.ui \
    Forms/Gen4/SearchCoinFlips.ui \
    Forms/Gen4/SeedtoTime4.ui \
    Forms/Gen4/Stationary4.ui \
    Forms/Gen4/Wild4.ui \
    Forms/Gen5/ProfileCalibrator5.ui \
    Forms/Gen5/ProfileEditor5.ui \
    Forms/Gen5/ProfileManager5.ui \
    Forms/Gen5/Stationary5.ui \
    Forms/MainWindow.ui \
    Forms/Util/EncounterLookup.ui \
    Forms/Util/IVCalculator.ui \
    Forms/Util/IVtoPID.ui \
    Forms/Util/Researcher.ui

HEADERS += \
    Core/Gen3/Egg3.hpp \
    Core/Gen3/EncounterArea3.hpp \
    Core/Gen3/Encounters3.hpp \
    Core/Gen3/Frame3.hpp \
    Core/Gen3/GameCubeRTCSearcher.hpp \
    Core/Gen3/GameCubeSeedSearcher.hpp \
    Core/Gen3/Generator3.hpp \
    Core/Gen3/IVSearcher3.hpp \
    Core/Gen3/LockInfo.hpp \
    Core/Gen3/Profile3.hpp \
    Core/Gen3/Searcher3.hpp \
    Core/Gen3/ShadowLock.hpp \
    Core/Gen4/ChainedSIDCalc.hpp \
    Core/Gen4/Egg4.hpp \
    Core/Gen4/EggSearcher4.hpp \
    Core/Gen4/EncounterArea4.hpp \
    Core/Gen4/Encounters4.hpp \
    Core/Gen4/Frame4.hpp \
    Core/Gen4/Generator4.hpp \
    Core/Gen4/HGSSRoamer.hpp \
    Core/Gen4/IDSearcher4.hpp \
    Core/Gen4/IVSearcher4.hpp \
    Core/Gen4/Profile4.hpp \
    Core/Gen4/Searcher4.hpp \
    Core/Gen5/DSType.hpp \
    Core/Gen5/Frame5.hpp \
    Core/Gen5/Generator5.hpp \
    Core/Gen5/Nazos.hpp \
    Core/Gen5/Profile5.hpp \
    Core/Parents/Egg.hpp \
    Core/Parents/EncounterArea.hpp \
    Core/Parents/Frame.hpp \
    Core/Parents/FrameCompare.hpp \
    Core/Parents/Generator.hpp \
    Core/Parents/Pokemon.hpp \
    Core/Parents/Profile.hpp \
    Core/Parents/Searcher.hpp \
    Core/Parents/Slot.hpp \
    Core/RNG/IRNG.hpp \
    Core/RNG/IRNG64.hpp \
    Core/RNG/LCRNG.hpp \
    Core/RNG/LCRNG64.hpp \
    Core/RNG/MTRNG.hpp \
    Core/RNG/RNGCache.hpp \
    Core/RNG/RNGEuclidean.hpp \
    Core/RNG/SFMT.hpp \
    Core/RNG/SHA1.hpp \
    Core/RNG/TinyMT.hpp \
    Core/Util/Characteristic.hpp \
    Core/Util/Encounter.hpp \
    Core/Util/EncounterSlot.hpp \
    Core/Util/Game.hpp \
    Core/Util/Global.hpp \
    Core/Util/IVChecker.hpp \
    Core/Util/Language.hpp \
    Core/Util/Lead.hpp \
    Core/Util/Method.hpp \
    Core/Util/Nature.hpp \
    Core/Util/Power.hpp \
    Core/Util/ShadowType.hpp \
    Core/Util/Translator.hpp \
    Core/Util/Utilities.hpp \
    Forms/Controls/CheckList.hpp \
    Forms/Controls/EggSettings.hpp \
    Forms/Controls/GraphicsPixmapItem.hpp \
    Forms/Controls/IVFilter.hpp \
    Forms/Controls/Label.hpp \
    Forms/Controls/TableView.hpp \
    Forms/Controls/TextBox.hpp \
    Forms/Gen3/Eggs3.hpp \
    Forms/Gen3/GameCube.hpp \
    Forms/Gen3/GameCubeRTC.hpp \
    Forms/Gen3/GameCubeSeedFinder.hpp \
    Forms/Gen3/IDs3.hpp \
    Forms/Gen3/JirachiPattern.hpp \
    Forms/Gen3/PIDtoIVs.hpp \
    Forms/Gen3/PokeSpot.hpp \
    Forms/Gen3/ProfileEditor3.hpp \
    Forms/Gen3/ProfileManager3.hpp \
    Forms/Gen3/SeedToTime3.hpp \
    Forms/Gen3/SpindaPainter.hpp \
    Forms/Gen3/Stationary3.hpp \
    Forms/Gen3/Wild3.hpp \
    Forms/Gen4/ChainedSID.hpp \
    Forms/Gen4/Eggs4.hpp \
    Forms/Gen4/IDs4.hpp \
    Forms/Gen4/ProfileEditor4.hpp \
    Forms/Gen4/ProfileManager4.hpp \
    Forms/Gen4/RoamerMap.hpp \
    Forms/Gen4/SearchCalls.hpp \
    Forms/Gen4/SearchCoinFlips.hpp \
    Forms/Gen4/SeedtoTime4.hpp \
    Forms/Gen4/Stationary4.hpp \
    Forms/Gen4/Wild4.hpp \
    Forms/Gen5/ProfileCalibrator5.hpp \
    Forms/Gen5/ProfileEditor5.hpp \
    Forms/Gen5/ProfileManager5.hpp \
    Forms/Gen5/Stationary5.hpp \
    Forms/MainWindow.hpp \
    Forms/Util/EncounterLookup.hpp \
    Forms/Util/IVCalculator.hpp \
    Forms/Util/IVtoPID.hpp \
    Forms/Util/Researcher.hpp \
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
    Models/Gen5/Profile5Model.hpp \
    Models/TableModel.hpp \
    Models/Util/ResearcherModel.hpp \
    Util/DateTime.hpp \
    Util/ResearcherFrame.hpp \
    Util/TableUtility.hpp

SOURCES += \
    Core/Gen3/Egg3.cpp \
    Core/Gen3/EncounterArea3.cpp \
    Core/Gen3/Encounters3.cpp \
    Core/Gen3/Frame3.cpp \
    Core/Gen3/GameCubeRTCSearcher.cpp \
    Core/Gen3/GameCubeSeedSearcher.cpp \
    Core/Gen3/Generator3.cpp \
    Core/Gen3/IVSearcher3.cpp \
    Core/Gen3/LockInfo.cpp \
    Core/Gen3/Profile3.cpp \
    Core/Gen3/Searcher3.cpp \
    Core/Gen3/ShadowLock.cpp \
    Core/Gen4/ChainedSIDCalc.cpp \
    Core/Gen4/Egg4.cpp \
    Core/Gen4/EggSearcher4.cpp \
    Core/Gen4/EncounterArea4.cpp \
    Core/Gen4/Encounters4.cpp \
    Core/Gen4/Frame4.cpp \
    Core/Gen4/Generator4.cpp \
    Core/Gen4/HGSSRoamer.cpp \
    Core/Gen4/IDSearcher4.cpp \
    Core/Gen4/IVSearcher4.cpp \
    Core/Gen4/Profile4.cpp \
    Core/Gen4/Searcher4.cpp \
    Core/Gen5/Frame5.cpp \
    Core/Gen5/Generator5.cpp \
    Core/Gen5/Nazos.cpp \
    Core/Gen5/Profile5.cpp \
    Core/Parents/Egg.cpp \
    Core/Parents/EncounterArea.cpp \
    Core/Parents/Frame.cpp \
    Core/Parents/FrameCompare.cpp \
    Core/Parents/Generator.cpp \
    Core/Parents/Pokemon.cpp \
    Core/Parents/Profile.cpp \
    Core/Parents/Searcher.cpp \
    Core/Parents/Slot.cpp \
    Core/RNG/LCRNG.cpp \
    Core/RNG/LCRNG64.cpp \
    Core/RNG/MTRNG.cpp \
    Core/RNG/RNGCache.cpp \
    Core/RNG/RNGEuclidean.cpp \
    Core/RNG/SFMT.cpp \
    Core/RNG/SHA1.cpp \
    Core/RNG/TinyMT.cpp \
    Core/Util/Characteristic.cpp \
    Core/Util/EncounterSlot.cpp \
    Core/Util/IVChecker.cpp \
    Core/Util/Nature.cpp \
    Core/Util/Power.cpp \
    Core/Util/Translator.cpp \
    Core/Util/Utilities.cpp \
    Forms/Controls/CheckList.cpp \
    Forms/Controls/EggSettings.cpp \
    Forms/Controls/GraphicsPixmapItem.cpp \
    Forms/Controls/IVFilter.cpp \
    Forms/Controls/Label.cpp \
    Forms/Controls/TableView.cpp \
    Forms/Controls/TextBox.cpp \
    Forms/Gen3/Eggs3.cpp \
    Forms/Gen3/GameCube.cpp \
    Forms/Gen3/GameCubeRTC.cpp \
    Forms/Gen3/GameCubeSeedFinder.cpp \
    Forms/Gen3/IDs3.cpp \
    Forms/Gen3/JirachiPattern.cpp \
    Forms/Gen3/PIDtoIVs.cpp \
    Forms/Gen3/PokeSpot.cpp \
    Forms/Gen3/ProfileEditor3.cpp \
    Forms/Gen3/ProfileManager3.cpp \
    Forms/Gen3/SeedToTime3.cpp \
    Forms/Gen3/SpindaPainter.cpp \
    Forms/Gen3/Stationary3.cpp \
    Forms/Gen3/Wild3.cpp \
    Forms/Gen4/ChainedSID.cpp \
    Forms/Gen4/Eggs4.cpp \
    Forms/Gen4/IDs4.cpp \
    Forms/Gen4/ProfileEditor4.cpp \
    Forms/Gen4/ProfileManager4.cpp \
    Forms/Gen4/RoamerMap.cpp \
    Forms/Gen4/SearchCalls.cpp \
    Forms/Gen4/SearchCoinFlips.cpp \
    Forms/Gen4/SeedtoTime4.cpp \
    Forms/Gen4/Stationary4.cpp \
    Forms/Gen4/Wild4.cpp \
    Forms/Gen5/ProfileCalibrator5.cpp \
    Forms/Gen5/ProfileEditor5.cpp \
    Forms/Gen5/ProfileManager5.cpp \
    Forms/Gen5/Stationary5.cpp \
    Forms/MainWindow.cpp \
    Forms/Util/EncounterLookup.cpp \
    Forms/Util/IVCalculator.cpp \
    Forms/Util/IVtoPID.cpp \
    Forms/Util/Researcher.cpp \
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
    Models/Gen5/Profile5Model.cpp \
    Models/Util/ResearcherModel.cpp \
    Util/DateTime.cpp \
    Util/ResearcherFrame.cpp \
    Util/TableUtility.cpp \
    main.cpp
