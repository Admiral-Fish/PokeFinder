lessThan(QT_MAJOR_VERSION, 5):error("You need at least Qt 5.12 to build PokeFinder")
equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 12):error("You need at least Qt 5.12 to build PokeFinder")

QT += concurrent network widgets
CONFIG += c++1z lrelease embed_translations
QMAKE_LRELEASE_FLAGS = -nounfinished -removeidentical

TARGET = PokeFinder
TEMPLATE = app
VERSION = 2.5.3
#GIT_VERSION = $$system(git rev-parse HEAD)
#GIT_BRANCH = $$system(git rev-parse --abbrev-ref HEAD)
QMAKE_TARGET_DESCRIPTION = PokeFinder
QMAKE_TARGET_COPYRIGHT = Admiral_Fish

RC_ICONS += Images/pokefinder.ico
ICON += Images/pokefinder.icns

DEFINES += VERSION=\\\"$$VERSION\\\"
#DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
#DEFINES += GIT_BRANCH=\\\"$$GIT_BRANCH\\\"
DEFINES += QT_DEPRECATED_WARNINGS

TRANSLATIONS += \
    i18n/PokeFinder_en.ts \
    i18n/PokeFinder_fr.ts \
    i18n/PokeFinder_es.ts \
    i18n/PokeFinder_de.ts \
    i18n/PokeFinder_it.ts \
    i18n/PokeFinder_ja.ts \
    i18n/PokeFinder_ko.ts \
    i18n/PokeFinder_zh.ts

RESOURCES += \
    QDarkStyleSheet/qdarkstyle/style.qrc \
    resources.qrc

FORMS += \
    Forms/Controls/EggSettings.ui \
    Forms/Controls/Filter.ui \
    Forms/Controls/IVFilter.ui \
    Forms/Gen3/Eggs3.ui \
    Forms/Gen3/GameCube.ui \
    Forms/Gen3/IDs3.ui \
    Forms/Gen3/Profile/ProfileEditor3.ui \
    Forms/Gen3/Profile/ProfileManager3.ui \
    Forms/Gen3/Stationary3.ui \
    Forms/Gen3/Tools/GameCubeRTC.ui \
    Forms/Gen3/Tools/GameCubeSeedFinder.ui \
    Forms/Gen3/Tools/JirachiPattern.ui \
    Forms/Gen3/Tools/PIDIV.ui \
    Forms/Gen3/Tools/PokeSpot.ui \
    Forms/Gen3/Tools/SeedTime3.ui \
    Forms/Gen3/Tools/SpindaPainter.ui \
    Forms/Gen3/Wild3.ui \
    Forms/Gen4/Eggs4.ui \
    Forms/Gen4/IDs4.ui \
    Forms/Gen4/Profile/ProfileEditor4.ui \
    Forms/Gen4/Profile/ProfileManager4.ui \
    Forms/Gen4/Stationary4.ui \
    Forms/Gen4/Tools/ChainedSID.ui \
    Forms/Gen4/Tools/RoamerMap.ui \
    Forms/Gen4/Tools/SearchCalls.ui \
    Forms/Gen4/Tools/SearchCoinFlips.ui \
    Forms/Gen4/Tools/SeedtoTime4.ui \
    Forms/Gen4/Wild4.ui \
    Forms/MainWindow.ui \
    Forms/Util/EncounterLookup.ui \
    Forms/Util/IVCalculator.ui \
    Forms/Util/IVtoPID.ui \
    Forms/Util/Researcher.ui

HEADERS += \
    Core/Enum/Encounter.hpp \
    Core/Enum/Game.hpp \
    Core/Enum/Language.hpp \
    Core/Enum/Lead.hpp \
    Core/Enum/Method.hpp \
    Core/Enum/ShadowType.hpp \
    Core/Gen3/EncounterArea3.hpp \
    Core/Gen3/Encounters3.hpp \
    Core/Gen3/Frames/EggFrame3.hpp \
    Core/Gen3/Frames/GameCubeFrame.hpp \
    Core/Gen3/Frames/GameCubeRTCFrame.hpp \
    Core/Gen3/Frames/IDFrame3.hpp \
    Core/Gen3/Frames/PIDIVFrame.hpp \
    Core/Gen3/Frames/SeedTimeFrame3.hpp \
    Core/Gen3/Generators/EggGenerator3.hpp \
    Core/Gen3/Generators/GameCubeGenerator.hpp \
    Core/Gen3/Generators/IDGenerator3.hpp \
    Core/Gen3/Generators/PokeSpotGenerator.hpp \
    Core/Gen3/Generators/StationaryGenerator3.hpp \
    Core/Gen3/Generators/WildGenerator3.hpp \
    Core/Gen3/LockInfo.hpp \
    Core/Gen3/Profile3.hpp \
    Core/Gen3/ProfileLoader3.hpp \
    Core/Gen3/Searchers/ChannelSeedSearcher.hpp \
    Core/Gen3/Searchers/ColoSeedSearcher.hpp \
    Core/Gen3/Searchers/GalesSeedSearcher.hpp \
    Core/Gen3/Searchers/GameCubeSearcher.hpp \
    Core/Gen3/Searchers/RTCSearcher.hpp \
    Core/Gen3/Searchers/SeedSearcher.hpp \
    Core/Gen3/Searchers/StationarySearcher3.hpp \
    Core/Gen3/Searchers/WildSearcher3.hpp \
    Core/Gen3/ShadowLock.hpp \
    Core/Gen3/Tools/JirachiPatternCalculator.hpp \
    Core/Gen3/Tools/PIDIVCalculator.hpp \
    Core/Gen3/Tools/SeedTimeCalculator3.hpp \
    Core/Gen4/EncounterArea4.hpp \
    Core/Gen4/Encounters4.hpp \
    Core/Gen4/Frames/EggFrame4.hpp \
    Core/Gen4/Frames/IDFrame4.hpp \
    Core/Gen4/Generators/EggGenerator4.hpp \
    Core/Gen4/Generators/IDGenerator4.hpp \
    Core/Gen4/Generators/StationaryGenerator4.hpp \
    Core/Gen4/Generators/WildGenerator4.hpp \
    Core/Gen4/HGSSRoamer.hpp \
    Core/Gen4/Profile4.hpp \
    Core/Gen4/ProfileLoader4.hpp \
    Core/Gen4/Searchers/EggSearcher4.hpp \
    Core/Gen4/Searchers/IDSearcher4.hpp \
    Core/Gen4/Searchers/StationarySearcher4.hpp \
    Core/Gen4/Searchers/WildSearcher4.hpp \
    Core/Gen4/Tools/ChainedSIDCalc.hpp \
    Core/Parents/EncounterArea.hpp \
    Core/Parents/Filters/FrameFilter.hpp \
    Core/Parents/Filters/IDFilter.hpp \
    Core/Parents/Frames/EggFrame.hpp \
    Core/Parents/Frames/Frame.hpp \
    Core/Parents/Frames/IDFrame.hpp \
    Core/Parents/Frames/ResearcherFrame.hpp \
    Core/Parents/Frames/StationaryFrame.hpp \
    Core/Parents/Frames/WildFrame.hpp \
    Core/Parents/Generators/EggGenerator.hpp \
    Core/Parents/Generators/Generator.hpp \
    Core/Parents/Generators/IDGenerator.hpp \
    Core/Parents/Generators/StationaryGenerator.hpp \
    Core/Parents/Generators/WildGenerator.hpp \
    Core/Parents/PersonalInfo.hpp \
    Core/Parents/Profile.hpp \
    Core/Parents/Searchers/Searcher.hpp \
    Core/Parents/Searchers/StationarySearcher.hpp \
    Core/Parents/Searchers/WildSearcher.hpp \
    Core/Parents/Slot.hpp \
    Core/RNG/IRNG.hpp \
    Core/RNG/LCRNG.hpp \
    Core/RNG/LCRNG64.hpp \
    Core/RNG/MTRNG.hpp \
    Core/RNG/RNGCache.hpp \
    Core/RNG/RNGEuclidean.hpp \
    Core/RNG/SFMT.hpp \
    Core/RNG/TinyMT.hpp \
    Core/Util/DateTime.hpp \
    Core/Util/EncounterSlot.hpp \
    Core/Util/Global.hpp \
    Core/Util/IVChecker.hpp \
    Core/Util/Nature.hpp \
    Core/Util/Translator.hpp \
    Core/Util/Utilities.hpp \
    Forms/Controls/CheckList.hpp \
    Forms/Controls/ComboBox.hpp \
    Forms/Controls/EggSettings.hpp \
    Forms/Controls/Filter.hpp \
    Forms/Controls/GraphicsPixmapItem.hpp \
    Forms/Controls/IVFilter.hpp \
    Forms/Controls/Label.hpp \
    Forms/Controls/TableView.hpp \
    Forms/Controls/TextBox.hpp \
    Forms/Gen3/Eggs3.hpp \
    Forms/Gen3/GameCube.hpp \
    Forms/Gen3/IDs3.hpp \
    Forms/Gen3/Profile/ProfileEditor3.hpp \
    Forms/Gen3/Profile/ProfileManager3.hpp \
    Forms/Gen3/Stationary3.hpp \
    Forms/Gen3/Tools/GameCubeRTC.hpp \
    Forms/Gen3/Tools/GameCubeSeedFinder.hpp \
    Forms/Gen3/Tools/JirachiPattern.hpp \
    Forms/Gen3/Tools/PIDIV.hpp \
    Forms/Gen3/Tools/PokeSpot.hpp \
    Forms/Gen3/Tools/SeedTime3.hpp \
    Forms/Gen3/Tools/SpindaPainter.hpp \
    Forms/Gen3/Wild3.hpp \
    Forms/Gen4/Eggs4.hpp \
    Forms/Gen4/IDs4.hpp \
    Forms/Gen4/Profile/ProfileEditor4.hpp \
    Forms/Gen4/Profile/ProfileManager4.hpp \
    Forms/Gen4/Stationary4.hpp \
    Forms/Gen4/Tools/ChainedSID.hpp \
    Forms/Gen4/Tools/RoamerMap.hpp \
    Forms/Gen4/Tools/SearchCalls.hpp \
    Forms/Gen4/Tools/SearchCoinFlips.hpp \
    Forms/Gen4/Tools/SeedtoTime4.hpp \
    Forms/Gen4/Wild4.hpp \
    Forms/MainWindow.hpp \
    Forms/Util/EncounterLookup.hpp \
    Forms/Util/IVCalculator.hpp \
    Forms/Util/IVtoPID.hpp \
    Forms/Util/Researcher.hpp \
    Models/Gen3/EggModel3.hpp \
    Models/Gen3/GameCubeModel.hpp \
    Models/Gen3/GameCubeRTCModel.hpp \
    Models/Gen3/IDModel3.hpp \
    Models/Gen3/PIDIVModel.hpp \
    Models/Gen3/PokeSpotModel.hpp \
    Models/Gen3/ProfileModel3.hpp \
    Models/Gen3/SeedTimeModel3.hpp \
    Models/Gen3/StationaryModel3.hpp \
    Models/Gen3/WildModel3.hpp \
    Models/Gen4/EggModel4.hpp \
    Models/Gen4/IDModel4.hpp \
    Models/Gen4/ProfileModel4.hpp \
    Models/Gen4/SeedtoTimeModel4.hpp \
    Models/Gen4/StationaryModel4.hpp \
    Models/Gen4/WildModel4.hpp \
    Models/TableModel.hpp \
    Models/Util/ResearcherModel.hpp

SOURCES += \
    Core/Gen3/EncounterArea3.cpp \
    Core/Gen3/Encounters3.cpp \
    Core/Gen3/Frames/EggFrame3.cpp \
    Core/Gen3/Frames/GameCubeFrame.cpp \
    Core/Gen3/Frames/GameCubeRTCFrame.cpp \
    Core/Gen3/Frames/IDFrame3.cpp \
    Core/Gen3/Frames/PIDIVFrame.cpp \
    Core/Gen3/Frames/SeedTimeFrame3.cpp \
    Core/Gen3/Generators/EggGenerator3.cpp \
    Core/Gen3/Generators/GameCubeGenerator.cpp \
    Core/Gen3/Generators/IDGenerator3.cpp \
    Core/Gen3/Generators/PokeSpotGenerator.cpp \
    Core/Gen3/Generators/StationaryGenerator3.cpp \
    Core/Gen3/Generators/WildGenerator3.cpp \
    Core/Gen3/LockInfo.cpp \
    Core/Gen3/Profile3.cpp \
    Core/Gen3/ProfileLoader3.cpp \
    Core/Gen3/Searchers/ChannelSeedSearcher.cpp \
    Core/Gen3/Searchers/ColoSeedSearcher.cpp \
    Core/Gen3/Searchers/GalesSeedSearcher.cpp \
    Core/Gen3/Searchers/GameCubeSearcher.cpp \
    Core/Gen3/Searchers/RTCSearcher.cpp \
    Core/Gen3/Searchers/SeedSearcher.cpp \
    Core/Gen3/Searchers/StationarySearcher3.cpp \
    Core/Gen3/Searchers/WildSearcher3.cpp \
    Core/Gen3/ShadowLock.cpp \
    Core/Gen3/Tools/JirachiPatternCalculator.cpp \
    Core/Gen3/Tools/PIDIVCalculator.cpp \
    Core/Gen3/Tools/SeedTimeCalculator3.cpp \
    Core/Gen4/EncounterArea4.cpp \
    Core/Gen4/Encounters4.cpp \
    Core/Gen4/Frames/EggFrame4.cpp \
    Core/Gen4/Frames/IDFrame4.cpp \
    Core/Gen4/Generators/EggGenerator4.cpp \
    Core/Gen4/Generators/IDGenerator4.cpp \
    Core/Gen4/Generators/StationaryGenerator4.cpp \
    Core/Gen4/Generators/WildGenerator4.cpp \
    Core/Gen4/HGSSRoamer.cpp \
    Core/Gen4/Profile4.cpp \
    Core/Gen4/ProfileLoader4.cpp \
    Core/Gen4/Searchers/EggSearcher4.cpp \
    Core/Gen4/Searchers/IDSearcher4.cpp \
    Core/Gen4/Searchers/StationarySearcher4.cpp \
    Core/Gen4/Searchers/WildSearcher4.cpp \
    Core/Gen4/Tools/ChainedSIDCalc.cpp \
    Core/Parents/EncounterArea.cpp \
    Core/Parents/Filters/FrameFilter.cpp \
    Core/Parents/Filters/IDFilter.cpp \
    Core/Parents/Frames/EggFrame.cpp \
    Core/Parents/Frames/Frame.cpp \
    Core/Parents/Frames/IDFrame.cpp \
    Core/Parents/Frames/ResearcherFrame.cpp \
    Core/Parents/Frames/StationaryFrame.cpp \
    Core/Parents/Frames/WildFrame.cpp \
    Core/Parents/Generators/EggGenerator.cpp \
    Core/Parents/Generators/Generator.cpp \
    Core/Parents/Generators/IDGenerator.cpp \
    Core/Parents/Generators/StationaryGenerator.cpp \
    Core/Parents/Generators/WildGenerator.cpp \
    Core/Parents/PersonalInfo.cpp \
    Core/Parents/Profile.cpp \
    Core/Parents/Searchers/Searcher.cpp \
    Core/Parents/Searchers/StationarySearcher.cpp \
    Core/Parents/Searchers/WildSearcher.cpp \
    Core/Parents/Slot.cpp \
    Core/RNG/MTRNG.cpp \
    Core/RNG/RNGCache.cpp \
    Core/RNG/RNGEuclidean.cpp \
    Core/RNG/SFMT.cpp \
    Core/RNG/TinyMT.cpp \
    Core/Util/DateTime.cpp \
    Core/Util/EncounterSlot.cpp \
    Core/Util/IVChecker.cpp \
    Core/Util/Nature.cpp \
    Core/Util/Translator.cpp \
    Core/Util/Utilities.cpp \
    Forms/Controls/CheckList.cpp \
    Forms/Controls/ComboBox.cpp \
    Forms/Controls/EggSettings.cpp \
    Forms/Controls/Filter.cpp \
    Forms/Controls/GraphicsPixmapItem.cpp \
    Forms/Controls/IVFilter.cpp \
    Forms/Controls/Label.cpp \
    Forms/Controls/TableView.cpp \
    Forms/Controls/TextBox.cpp \
    Forms/Gen3/Eggs3.cpp \
    Forms/Gen3/GameCube.cpp \
    Forms/Gen3/IDs3.cpp \
    Forms/Gen3/Profile/ProfileEditor3.cpp \
    Forms/Gen3/Profile/ProfileManager3.cpp \
    Forms/Gen3/Stationary3.cpp \
    Forms/Gen3/Tools/GameCubeRTC.cpp \
    Forms/Gen3/Tools/GameCubeSeedFinder.cpp \
    Forms/Gen3/Tools/JirachiPattern.cpp \
    Forms/Gen3/Tools/PIDIV.cpp \
    Forms/Gen3/Tools/PokeSpot.cpp \
    Forms/Gen3/Tools/SeedTime3.cpp \
    Forms/Gen3/Tools/SpindaPainter.cpp \
    Forms/Gen3/Wild3.cpp \
    Forms/Gen4/Eggs4.cpp \
    Forms/Gen4/IDs4.cpp \
    Forms/Gen4/Profile/ProfileEditor4.cpp \
    Forms/Gen4/Profile/ProfileManager4.cpp \
    Forms/Gen4/Stationary4.cpp \
    Forms/Gen4/Tools/ChainedSID.cpp \
    Forms/Gen4/Tools/RoamerMap.cpp \
    Forms/Gen4/Tools/SearchCalls.cpp \
    Forms/Gen4/Tools/SearchCoinFlips.cpp \
    Forms/Gen4/Tools/SeedtoTime4.cpp \
    Forms/Gen4/Wild4.cpp \
    Forms/MainWindow.cpp \
    Forms/Util/EncounterLookup.cpp \
    Forms/Util/IVCalculator.cpp \
    Forms/Util/IVtoPID.cpp \
    Forms/Util/Researcher.cpp \
    Models/Gen3/EggModel3.cpp \
    Models/Gen3/GameCubeModel.cpp \
    Models/Gen3/GameCubeRTCModel.cpp \
    Models/Gen3/IDModel3.cpp \
    Models/Gen3/PIDIVModel.cpp \
    Models/Gen3/PokeSpotModel.cpp \
    Models/Gen3/ProfileModel3.cpp \
    Models/Gen3/SeedTimeModel3.cpp \
    Models/Gen3/StationaryModel3.cpp \
    Models/Gen3/WildModel3.cpp \
    Models/Gen4/EggModel4.cpp \
    Models/Gen4/IDModel4.cpp \
    Models/Gen4/ProfileModel4.cpp \
    Models/Gen4/SeedtoTimeModel4.cpp \
    Models/Gen4/StationaryModel4.cpp \
    Models/Gen4/WildModel4.cpp \
    Models/Util/ResearcherModel.cpp \
    main.cpp
