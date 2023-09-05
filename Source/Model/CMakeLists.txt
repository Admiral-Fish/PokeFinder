project(PokeFinderModel)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(PokeFinderModel STATIC
    Gen3/IDModel3.cpp
    Gen3/IDModel3.hpp
    Gen3/EggModel3.cpp
    Gen3/EggModel3.hpp
    Gen3/GameCubeModel.cpp
    Gen3/GameCubeModel.hpp
    Gen3/PIDToIVModel.cpp
    Gen3/PIDToIVModel.hpp
    Gen3/PokeSpotModel.cpp
    Gen3/PokeSpotModel.hpp
    Gen3/ProfileModel3.cpp
    Gen3/ProfileModel3.hpp
    Gen3/SeedToTimeModel3.cpp
    Gen3/SeedToTimeModel3.hpp
    Gen3/StaticModel3.cpp
    Gen3/StaticModel3.hpp
    Gen3/WildModel3.cpp
    Gen3/WildModel3.hpp
    Gen4/EggModel4.cpp
    Gen4/EggModel4.hpp
    Gen4/EventModel4.cpp
    Gen4/EventModel4.hpp
    Gen4/IDModel4.cpp
    Gen4/IDModel4.hpp
    Gen4/ProfileModel4.cpp
    Gen4/ProfileModel4.hpp
    Gen4/SeedToTimeModel4.cpp
    Gen4/SeedToTimeModel4.hpp
    Gen4/StaticModel4.cpp
    Gen4/StaticModel4.hpp
    Gen4/WildModel4.cpp
    Gen4/WildModel4.hpp
    Gen5/DreamRadarModel.cpp
    Gen5/DreamRadarModel.hpp
    Gen5/EggModel5.cpp
    Gen5/EggModel5.hpp
    Gen5/EventModel5.cpp
    Gen5/EventModel5.hpp
    Gen5/HiddenGrottoModel.cpp
    Gen5/HiddenGrottoModel.hpp
    Gen5/IDModel5.cpp
    Gen5/IDModel5.hpp
    Gen5/ProfileModel5.cpp
    Gen5/ProfileModel5.hpp
    Gen5/ProfileSearcherModel5.cpp
    Gen5/ProfileSearcherModel5.hpp
    Gen5/StaticModel5.cpp
    Gen5/StaticModel5.hpp
    Gen5/WildModel5.cpp
    Gen5/WildModel5.hpp
    Gen8/EggModel8.cpp
    Gen8/EggModel8.hpp
    Gen8/IDModel8.cpp
    Gen8/IDModel8.hpp
    Gen8/ProfileModel8.cpp
    Gen8/ProfileModel8.hpp
    Gen8/RaidModel.cpp
    Gen8/RaidModel.hpp
    Gen8/StaticModel8.cpp
    Gen8/StaticModel8.hpp
    Gen8/UndergroundModel.cpp
    Gen8/UndergroundModel.hpp
    Gen8/WildModel8.cpp
    Gen8/WildModel8.hpp
    TableModel.hpp
    Util/IVToPIDModel.cpp
    Util/IVToPIDModel.hpp
    Util/ResearcherModel.cpp
    Util/ResearcherModel.hpp
)

target_link_libraries(PokeFinderModel PRIVATE PokeFinderCore Qt6::Core)
