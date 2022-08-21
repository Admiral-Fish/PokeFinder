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
    TableModel.hpp
)

target_link_libraries(PokeFinderModel PRIVATE PokeFinderCore Qt6::Core)
