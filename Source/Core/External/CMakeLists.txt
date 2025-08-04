#zstd
set(ZSTD_LEGACY_SUPPORT OFF)
set(ZSTD_BUILD_STATIC ON)
set(ZSTD_BUILD_SHARED OFF)
add_subdirectory(zstd/build/cmake zstd)
target_include_directories(zstd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/zstd/lib/)
