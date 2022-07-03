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
    Gen3/Generators/IDGenerator3.cpp
    Gen3/Profile3.cpp
    Gen4/Profile4.cpp
    Gen5/Profile5.cpp
    Gen8/Profile8.cpp
    Parents/Filters/IDFilter.cpp
    Parents/Filters/StateFilter.cpp
    Parents/PersonalLoader.cpp
    Parents/Profile.cpp
    Parents/ProfileLoader.cpp
    RNG/MT.cpp
    RNG/RNGCache.cpp
    RNG/RNGEuclidean.cpp
    RNG/SFMT.cpp
    RNG/SHA1.cpp
    RNG/TinyMT.cpp
    RNG/Xoroshiro.cpp
    RNG/Xorshift.cpp
    Util/DateTime.cpp
    Util/Translator.cpp
    Util/Utilities.cpp
)

if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/Resources/Encounters.hpp)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_custom_target(
        Resources ALL
        COMMAND ${PYTHON3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/Resources/embed.py
    )

    add_dependencies(PokeFinderCore Resources)
endif ()
