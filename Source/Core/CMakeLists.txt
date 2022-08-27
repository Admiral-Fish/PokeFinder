project(PokeFinderCore)

include_directories("${CMAKE_SOURCE_DIR}/External")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/Zc:inline)
endif ()

if (UNIX)
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
    Enum/Headbutt.hpp
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
    Gen3/Generators/IDGenerator3.cpp
    Gen3/Generators/IDGenerator3.hpp
    Gen3/Generators/StaticGenerator3.cpp
    Gen3/Generators/StaticGenerator3.hpp
    Gen3/Generators/WildGenerator3.cpp
    Gen3/Generators/WildGenerator3.hpp
    Gen3/Searchers/StaticSearcher3.cpp
    Gen3/Searchers/StaticSearcher3.hpp
    Gen3/Searchers/WildSearcher3.cpp
    Gen3/Searchers/WildSearcher3.hpp
    Gen3/Profile3.cpp
    Gen3/Profile3.hpp
    Gen3/States/State3.cpp
    Gen3/States/State3.hpp
    Gen3/States/WildState3.cpp
    Gen3/States/WildState3.hpp
    Gen4/Profile4.cpp
    Gen4/Profile4.hpp
    Gen5/Profile5.cpp
    Gen5/Profile5.hpp
    Gen8/Den.hpp
    Gen8/Profile8.cpp
    Gen8/Profile8.hpp
    Gen8/Raid.hpp
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
    RNG/LCRNG.hpp
    RNG/LCRNG64.hpp
    RNG/MT.cpp
    RNG/MT.hpp
    RNG/MTFast.hpp
    RNG/RNGCache.cpp
    RNG/RNGCache.hpp
    RNG/RNGEuclidean.cpp
    RNG/RNGEuclidean.hpp
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

target_link_libraries(PokeFinderCore bz2_static)

if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/Resources/Encounters.hpp)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_custom_target(
        Resources ALL
        COMMAND ${PYTHON3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/Resources/embed.py
    )

    add_dependencies(PokeFinderCore Resources)
endif ()
