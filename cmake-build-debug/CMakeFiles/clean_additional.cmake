# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "Source/Forms/CMakeFiles/PokeFinder_autogen.dir/AutogenUsed.txt"
  "Source/Forms/CMakeFiles/PokeFinder_autogen.dir/ParseCache.txt"
  "Source/Forms/PokeFinder_autogen"
  )
endif()
