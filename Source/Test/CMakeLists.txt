project(PokeFinderTest LANGUAGES CXX)

find_package(Qt6 COMPONENTS Core Test REQUIRED)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/enum_map.py)

add_executable(PokeFinderTest
    Data.cpp
    Data.hpp
    data.qrc
    Enum.hpp
    Gen3/EggGenerator3Test.cpp
    Gen3/EggGenerator3Test.hpp
    Gen3/GameCubeGeneratorTest.cpp
    Gen3/GameCubeGeneratorTest.hpp
    Gen3/GameCubeSearcherTest.cpp
    Gen3/GameCubeSearcherTest.hpp
    Gen3/IDGenerator3Test.cpp
    Gen3/IDGenerator3Test.hpp
    Gen3/JirachiPatternTest.cpp
    Gen3/JirachiPatternTest.hpp
    Gen3/PIDToIVCalculatorTest.cpp
    Gen3/PIDToIVCalculatorTest.hpp
    Gen3/PokeSpotGeneratorTest.cpp
    Gen3/PokeSpotGeneratorTest.hpp
    Gen3/SeedToTimeCalculator3Test.cpp
    Gen3/SeedToTimeCalculator3Test.hpp
    Gen3/StaticGenerator3Test.cpp
    Gen3/StaticGenerator3Test.hpp
    Gen3/StaticSearcher3Test.cpp
    Gen3/StaticSearcher3Test.hpp
    Gen3/WildGenerator3Test.cpp
    Gen3/WildGenerator3Test.hpp
    Gen3/WildSearcher3Test.cpp
    Gen3/WildSearcher3Test.hpp
    Gen4/ChainedSIDCalcTest.cpp
    Gen4/ChainedSIDCalcTest.hpp
    Gen4/EggGenerator4Test.cpp
    Gen4/EggGenerator4Test.hpp
    Gen4/IDGenerator4Test.cpp
    Gen4/IDGenerator4Test.hpp
    Gen4/IDSearcher4Test.cpp
    Gen4/IDSearcher4Test.hpp
    Gen4/SeedToTimeCalculator4Test.cpp
    Gen4/SeedToTimeCalculator4Test.hpp
    Gen4/StaticGenerator4Test.cpp
    Gen4/StaticGenerator4Test.hpp
    Gen4/StaticSearcher4Test.cpp
    Gen4/StaticSearcher4Test.hpp
    Gen4/WildGenerator4Test.cpp
    Gen4/WildGenerator4Test.hpp
    Gen4/WildSearcher4Test.cpp
    Gen4/WildSearcher4Test.hpp
    Gen5/DreamRadarGeneratorTest.cpp
    Gen5/DreamRadarGeneratorTest.hpp
    Gen5/EggGenerator5Test.cpp
    Gen5/EggGenerator5Test.hpp
    Gen5/EventGenerator5Test.cpp
    Gen5/EventGenerator5Test.hpp
    Gen5/HiddenGrottoGeneratorTest.cpp
    Gen5/HiddenGrottoGeneratorTest.hpp
    Gen5/IDGenerator5Test.cpp
    Gen5/IDGenerator5Test.hpp
    Gen5/ProfileSearcher5Test.cpp
    Gen5/ProfileSearcher5Test.hpp
    Gen8/EggGenerator8Test.cpp
    Gen8/EggGenerator8Test.hpp
    Gen8/EventGenerator8Test.cpp
    Gen8/EventGenerator8Test.hpp
    Gen8/IDGenerator8Test.cpp
    Gen8/IDGenerator8Test.hpp
    Gen8/RaidGeneratorTest.cpp
    Gen8/RaidGeneratorTest.hpp
    Gen8/StaticGenerator8Test.cpp
    Gen8/StaticGenerator8Test.hpp
    Gen8/UndergroundGeneratorTest.cpp
    Gen8/UndergroundGeneratorTest.hpp
    Gen8/WildGenerator8Test.cpp
    Gen8/WildGenerator8Test.hpp
    main.cpp
    RNG/LCRNGTest.cpp
    RNG/LCRNGTest.hpp
    RNG/LCRNGReverseTest.cpp
    RNG/LCRNGReverseTest.hpp
    RNG/LCRNG64Test.cpp
    RNG/LCRNG64Test.hpp
    RNG/MTTest.cpp
    RNG/MTTest.hpp
    RNG/SFMTTest.cpp
    RNG/SFMTTest.hpp
    RNG/SHA1Test.cpp
    RNG/SHA1Test.hpp
    RNG/TinyMTTest.cpp
    RNG/TinyMTTest.hpp
    RNG/XoroshiroTest.cpp
    RNG/XoroshiroTest.hpp
    RNG/XorshiftTest.cpp
    RNG/XorshiftTest.hpp
    Util/DateTimeTest.cpp
    Util/DateTimeTest.hpp
    Util/EncounterSlotTest.cpp
    Util/EncounterSlotTest.hpp
    Util/IVCheckerTest.cpp
    Util/IVCheckerTest.hpp
    Util/IVToPIDCalculatorTest.cpp
    Util/IVToPIDCalculatorTest.hpp
)

add_test(NAME Test COMMAND PokeFinderTest)

target_link_libraries(PokeFinderTest PRIVATE PokeFinderCore Qt6::Core Qt6::Test)
