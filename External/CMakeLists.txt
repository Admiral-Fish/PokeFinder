#bzip2
set(ENABLE_LIB_ONLY ON)
set(ENABLE_STATIC_LIB ON)
set(ENABLE_SHARED_LIB OFF)
add_subdirectory(bzip2)
target_include_directories(bz2_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/bzip2)
