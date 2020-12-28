function(get_target_arch FLAG)
  execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -dumpmachine
    RESULT_VARIABLE RESULT
    OUTPUT_VARIABLE ARCH
    ERROR_QUIET
  )
  if (RESULT)
    message(FATAL_ERROR "Failed  to determine target architecture: ${RESULT}")
  else()
    string(REGEX MATCH "([^-]+).*" ARCH_MATCH ${ARCH})
    set(ARCH ${CMAKE_MATCH_1})
  endif()
  message(STATUS "Target arch: ${ARCH}")
  set(${FLAG} ${ARCH} PARENT_SCOPE)
endfunction()
