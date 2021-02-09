project(Tests LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(Tests
    RNG/LCRNGTest.cpp
    RNG/LCRNG64Test.cpp
    RNG/MTTest.cpp
    RNG/RNGCacheTest.cpp
    RNG/RNGEuclideanTest.cpp
    RNG/SFMTTest.cpp
    RNG/SHA1Test.cpp
    RNG/TinyMTTest.cpp
    main.cpp
)

add_test(NAME Tests COMMAND Tests)

target_link_libraries(Tests PRIVATE Core Qt5::Core Qt5::Test)

