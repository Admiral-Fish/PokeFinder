project(Core)

include(GetTargetArch)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX)
    get_target_arch(ARCH)
    if ((ARCH STREQUAL "x86_64") OR (ARCH STREQUAL "i686"))
        add_compile_options(-msse4.1)
    elseif (ARCH STREQUAL "arm")
        add_compile_options(-mfpu=neon)
    endif ()
endif ()

add_library(Core STATIC
    Gen3/EncounterArea3.cpp
    Gen3/Encounters3.cpp
    Gen3/Generators/EggGenerator3.cpp
    Gen3/Generators/GameCubeGenerator.cpp
    Gen3/Generators/IDGenerator3.cpp
    Gen3/Generators/PokeSpotGenerator.cpp
    Gen3/Generators/StationaryGenerator3.cpp
    Gen3/Generators/WildGenerator3.cpp
    Gen3/LockInfo.cpp
    Gen3/Profile3.cpp
    Gen3/Searchers/ChannelSeedSearcher.cpp
    Gen3/Searchers/ColoSeedSearcher.cpp
    Gen3/Searchers/GalesSeedSearcher.cpp
    Gen3/Searchers/GameCubeSearcher.cpp
    Gen3/Searchers/RTCSearcher.cpp
    Gen3/Searchers/SeedSearcher.cpp
    Gen3/Searchers/StationarySearcher3.cpp
    Gen3/Searchers/WildSearcher3.cpp
    Gen3/ShadowLock.cpp
    Gen3/States/EggState3.cpp
    Gen3/States/GameCubeRTCState.cpp
    Gen3/States/GameCubeState.cpp
    Gen3/States/IDState3.cpp
    Gen3/States/PIDIVState.cpp
    Gen3/States/SeedTimeState3.cpp
    Gen3/Tools/JirachiPatternCalculator.cpp
    Gen3/Tools/PIDIVCalculator.cpp
    Gen3/Tools/SeedTimeCalculator3.cpp
    Gen4/EncounterArea4.cpp
    Gen4/Encounters4.cpp
    Gen4/Generators/EggGenerator4.cpp
    Gen4/Generators/IDGenerator4.cpp
    Gen4/Generators/StationaryGenerator4.cpp
    Gen4/Generators/WildGenerator4.cpp
    Gen4/HGSSRoamer.cpp
    Gen4/Profile4.cpp
    Gen4/Searchers/EggSearcher4.cpp
    Gen4/Searchers/IDSearcher4.cpp
    Gen4/Searchers/StationarySearcher4.cpp
    Gen4/Searchers/WildSearcher4.cpp
    Gen4/SeedTime.cpp
    Gen4/States/EggState4.cpp
    Gen4/States/IDState4.cpp
    Gen4/Tools/ChainedSIDCalc.cpp
    Gen5/EncounterArea5.cpp
    Gen5/Encounters5.cpp
    Gen5/Filters/HiddenGrottoFilter.cpp
    Gen5/Generators/DreamRadarGenerator.cpp
    Gen5/Generators/EggGenerator5.cpp
    Gen5/Generators/EventGenerator5.cpp
    Gen5/Generators/HiddenGrottoGenerator.cpp
    Gen5/Generators/IDGenerator5.cpp
    Gen5/Generators/StationaryGenerator5.cpp
    Gen5/Keypresses.cpp
    Gen5/Nazos.cpp
    Gen5/PGF.cpp
    Gen5/Profile5.cpp
    Gen5/Searchers/DreamRadarSearcher.cpp
    Gen5/Searchers/EggSearcher5.cpp
    Gen5/Searchers/EventSearcher5.cpp
    Gen5/Searchers/IDSearcher5.cpp
    Gen5/Searchers/ProfileSearcher5.cpp
    Gen5/Searchers/StationarySearcher5.cpp
    Gen5/States/IDState5.cpp
    Gen5/States/ProfileSearcherState5.cpp
    Parents/Daycare.cpp
    Parents/EncounterArea.cpp
    Parents/Filters/IDFilter.cpp
    Parents/Filters/StateFilter.cpp
    Parents/Generators/EggGenerator.cpp
    Parents/Generators/Generator.cpp
    Parents/Generators/IDGenerator.cpp
    Parents/Generators/StationaryGenerator.cpp
    Parents/Generators/WildGenerator.cpp
    Parents/PersonalInfo.cpp
    Parents/Profile.cpp
    Parents/ProfileLoader.cpp
    Parents/Searchers/Searcher.cpp
    Parents/Searchers/StationarySearcher.cpp
    Parents/Searchers/WildSearcher.cpp
    Parents/Slot.cpp
    Parents/States/EggState.cpp
    Parents/States/IDState.cpp
    Parents/States/ResearcherState.cpp
    Parents/States/State.cpp
    Parents/States/StationaryState.cpp
    Parents/States/WildState.cpp
    RNG/MT.cpp
    RNG/RNGCache.cpp
    RNG/RNGEuclidean.cpp
    RNG/SFMT.cpp
    RNG/SHA1.cpp
    RNG/TinyMT.cpp
    Util/DateTime.cpp
    Util/EncounterSlot.cpp
    Util/IVChecker.cpp
    Util/Nature.cpp
    Util/Translator.cpp
    Util/Utilities.cpp
)
