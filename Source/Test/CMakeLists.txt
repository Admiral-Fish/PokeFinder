project(PokeFinderTest LANGUAGES CXX)

include_directories("${CMAKE_SOURCE_DIR}/External")

find_package(Qt6 COMPONENTS Core Test REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(PokeFinderTest
    Data.cpp
    Data.hpp
    data.qrc
    Gen3/IDGenerator3Test.cpp
    Gen3/IDGenerator3Test.hpp
    Gen3/StaticGenerator3Test.cpp
    Gen3/StaticGenerator3Test.hpp
    Gen3/StaticSearcher3Test.cpp
    Gen3/StaticSearcher3Test.hpp
    main.cpp
    RNG/LCRNGTest.cpp
    RNG/LCRNGTest.hpp
    RNG/LCRNG64Test.cpp
    RNG/LCRNG64Test.hpp
    RNG/MTTest.cpp
    RNG/MTTest.hpp
    RNG/RNGCacheTest.cpp
    RNG/RNGCacheTest.hpp
    RNG/RNGEuclideanTest.cpp
    RNG/RNGEuclideanTest.hpp
    RNG/SFMTTest.cpp
    RNG/SFMTTest.hpp
    #RNG/SHA1Test.cpp
    #RNG/SHA1Test.hpp
    RNG/TinyMTTest.cpp
    RNG/TinyMTTest.hpp
    Util/DateTimeTest.cpp
    Util/DateTimeTest.hpp
    #Util/EncounterSlotTest.cpp
    #Util/IVCheckerTest.cpp
)

add_test(NAME Test COMMAND PokeFinderTest)

target_link_libraries(PokeFinderTest PRIVATE PokeFinderCore Qt6::Core Qt6::Test)
