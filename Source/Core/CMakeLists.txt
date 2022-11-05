project(PokeFinderCore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(SIMD "Enable SIMD" ON)

if (SIMD)
    add_definitions(-DSIMD)
endif ()

if (MSVC)
    add_compile_options(/Zc:inline)
endif ()

if (UNIX AND SIMD)
    include(GetTargetArch)
    get_target_arch(ARCH)
    if ((ARCH STREQUAL "x86_64") OR (ARCH STREQUAL "i686"))
        add_compile_options(-msse4.1)
    elseif (ARCH STREQUAL "arm")
        add_compile_options(-mfpu=neon)
    endif ()
endif ()

add_library(PokeFinderCore STATIC
    Enum/Buttons.hpp
    Enum/DSType.hpp
    Enum/Encounter.hpp
    Enum/Game.hpp
    Enum/Language.hpp
    Enum/Lead.hpp
    Enum/Method.hpp
    Enum/ShadowType.hpp
    Enum/Shiny.hpp
    Gen3/EncounterArea3.cpp
    Gen3/EncounterArea3.hpp
    Gen3/Encounters3.cpp
    Gen3/Encounters3.hpp
    Gen3/Filters/StateFilter3.cpp
    Gen3/Filters/StateFilter3.hpp
    Gen3/Generators/GameCubeGenerator.cpp
    Gen3/Generators/GameCubeGenerator.hpp
    Gen3/Generators/IDGenerator3.cpp
    Gen3/Generators/IDGenerator3.hpp
    Gen3/Generators/StaticGenerator3.cpp
    Gen3/Generators/StaticGenerator3.hpp
    Gen3/Generators/WildGenerator3.cpp
    Gen3/Generators/WildGenerator3.hpp
    Gen3/LockInfo.hpp
    Gen3/Searchers/ChannelSeedSearcher.cpp
    Gen3/Searchers/ChannelSeedSearcher.hpp
    Gen3/Searchers/ColoSeedSearcher.cpp
    Gen3/Searchers/ColoSeedSearcher.hpp
    Gen3/Searchers/GalesSeedSearcher.cpp
    Gen3/Searchers/GalesSeedSearcher.hpp
    Gen3/Searchers/GameCubeSearcher.cpp
    Gen3/Searchers/GameCubeSearcher.hpp
    Gen3/Searchers/StaticSearcher3.cpp
    Gen3/Searchers/StaticSearcher3.hpp
    Gen3/Searchers/WildSearcher3.cpp
    Gen3/Searchers/WildSearcher3.hpp
    Gen3/ShadowLock.cpp
    Gen3/ShadowLock.hpp
    Gen3/ShadowTemplate.hpp
    Gen3/Profile3.cpp
    Gen3/Profile3.hpp
    Gen3/States/State3.cpp
    Gen3/States/State3.hpp
    Gen3/States/WildState3.cpp
    Gen3/States/WildState3.hpp
    Gen4/EncounterArea4.cpp
    Gen4/EncounterArea4.hpp
    Gen4/Encounters4.cpp
    Gen4/Encounters4.hpp
    Gen4/Filters/StateFilter4.cpp
    Gen4/Filters/StateFilter4.hpp
    Gen4/Generators/IDGenerator4.cpp
    Gen4/Generators/IDGenerator4.hpp
    Gen4/Generators/StaticGenerator4.cpp
    Gen4/Generators/StaticGenerator4.hpp
    Gen4/Generators/WildGenerator4.cpp
    Gen4/Generators/WildGenerator4.hpp
    Gen4/HGSSRoamer.cpp
    Gen4/HGSSRoamer.hpp
    Gen4/Profile4.cpp
    Gen4/Profile4.hpp
    Gen4/SeedTime4.cpp
    Gen4/SeedTime4.hpp
    Gen4/Searchers/IDSearcher4.cpp
    Gen4/Searchers/IDSearcher4.hpp
    Gen4/Searchers/StaticSearcher4.cpp
    Gen4/Searchers/StaticSearcher4.hpp
    Gen4/Searchers/WildSearcher4.cpp
    Gen4/Searchers/WildSearcher4.hpp
    Gen4/States/IDState4.hpp
    Gen4/States/State4.cpp
    Gen4/States/State4.hpp
    Gen4/States/WildState4.cpp
    Gen4/States/WildState4.hpp
    Gen4/StaticTemplate4.hpp
    Gen5/Nazos.cpp
    Gen5/Nazos.hpp
    Gen5/Keypresses.cpp
    Gen5/Keypresses.hpp
    Gen5/Profile5.cpp
    Gen5/Profile5.hpp
    Gen8/Den.hpp
    Gen8/EncounterArea8.hpp
    Gen8/Encounters8.cpp
    Gen8/Encounters8.hpp
    Gen8/Filters/StateFilter8.cpp
    Gen8/Filters/StateFilter8.hpp
    Gen8/Generators/EventGenerator8.cpp
    Gen8/Generators/EventGenerator8.hpp
    Gen8/Generators/IDGenerator8.cpp
    Gen8/Generators/IDGenerator8.hpp
    Gen8/Generators/StaticGenerator8.cpp
    Gen8/Generators/StaticGenerator8.hpp
    Gen8/Generators/WildGenerator8.cpp
    Gen8/Generators/WildGenerator8.hpp
    Gen8/Profile8.cpp
    Gen8/Profile8.hpp
    Gen8/Raid.hpp
    Gen8/States/IDState8.hpp
    Gen8/States/State8.cpp
    Gen8/States/State8.hpp
    Gen8/States/WildState8.cpp
    Gen8/States/WildState8.hpp
    Gen8/StaticTemplate8.hpp
    Gen8/WB8.hpp
    Parents/Daycare.hpp
    Parents/Filters/IDFilter.cpp
    Parents/Filters/IDFilter.hpp
    Parents/Filters/StateFilter.cpp
    Parents/Filters/StateFilter.hpp
    Parents/EncounterArea.cpp
    Parents/EncounterArea.hpp
    Parents/Generators/EggGenerator.hpp
    Parents/Generators/Generator.hpp
    Parents/Generators/IDGenerator.hpp
    Parents/Generators/StaticGenerator.hpp
    Parents/Generators/WildGenerator.hpp
    Parents/PersonalLoader.cpp
    Parents/PersonalLoader.hpp
    Parents/Profile.cpp
    Parents/Profile.hpp
    Parents/ProfileLoader.cpp
    Parents/ProfileLoader.hpp
    Parents/Searchers/Searcher.hpp
    Parents/Searchers/StaticSearcher.hpp
    Parents/Searchers/WildSearcher.hpp
    Parents/Slot.hpp
    Parents/States/EggState.hpp
    Parents/States/IDState.hpp
    Parents/States/ResearcherState.hpp
    Parents/States/State.hpp
    Parents/States/WildState.hpp
    Parents/StaticTemplate.hpp
    RNG/LCRNG.cpp
    RNG/LCRNG.hpp
    RNG/LCRNGReverse.cpp
    RNG/LCRNGReverse.hpp
    RNG/LCRNG64.cpp
    RNG/LCRNG64.hpp
    RNG/MT.cpp
    RNG/MT.hpp
    RNG/MTFast.hpp
    RNG/RNGList.hpp
    RNG/SFMT.cpp
    RNG/SFMT.hpp
    RNG/SHA1.cpp
    RNG/SHA1.hpp
    RNG/SIMD.hpp
    RNG/TinyMT.cpp
    RNG/TinyMT.hpp
    RNG/Xoroshiro.cpp
    RNG/Xoroshiro.hpp
    RNG/Xorshift.cpp
    RNG/Xorshift.hpp
    Util/DateTime.cpp
    Util/DateTime.hpp
    Util/EncounterSlot.cpp
    Util/EncounterSlot.hpp
    Util/IVChecker.cpp
    Util/IVChecker.hpp
    Util/Nature.cpp
    Util/Nature.hpp
    Util/Translator.cpp
    Util/Translator.hpp
    Util/Utilities.cpp
    Util/Utilities.hpp
)

target_include_directories(PokeFinderCore PUBLIC "${CMAKE_SOURCE_DIR}/External")
target_link_libraries(PokeFinderCore bz2_static)

if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/Resources/Encounters.hpp)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_custom_target(
        Resources ALL
        COMMAND ${PYTHON3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/Resources/embed.py
    )

    add_dependencies(PokeFinderCore Resources)
endif ()
