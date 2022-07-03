include_directories(.)

add_subdirectory(Core)
if (TEST)
    message("Building tests")
    add_subdirectory(Test)
endif ()
add_subdirectory(Model)
add_subdirectory(Form)
