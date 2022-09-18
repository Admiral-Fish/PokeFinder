project(PokeFinderModel)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(PokeFinderModel STATIC
    Gen3/IDModel3.cpp
    Gen3/IDModel3.hpp
    Gen3/ProfileModel3.cpp
    Gen3/ProfileModel3.hpp
    Gen3/StaticModel3.cpp
    Gen3/StaticModel3.hpp
    Gen3/WildModel3.cpp
    Gen3/WildModel3.hpp
    Gen4/IDModel4.cpp
    Gen4/IDModel4.hpp
    Gen4/ProfileModel4.cpp
    Gen4/ProfileModel4.hpp
    Gen4/StaticModel4.cpp
    Gen4/StaticModel4.hpp
    Gen4/WildModel4.cpp
    Gen4/WildModel4.hpp
    TableModel.hpp
    Util/ResearcherModel.cpp
    Util/ResearcherModel.hpp
)

target_link_libraries(PokeFinderModel PRIVATE PokeFinderCore Qt6::Core)
