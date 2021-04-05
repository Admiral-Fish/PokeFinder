include_directories(.)

add_subdirectory(Core)
if (TEST)
    message("Building tests")
    add_subdirectory(Tests)
endif ()
add_subdirectory(Forms)
