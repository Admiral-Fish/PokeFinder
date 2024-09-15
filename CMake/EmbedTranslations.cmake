function(embed_translations res_file)
  set(_qm_files ${ARGN})
  set(_res_file ${CMAKE_CURRENT_BINARY_DIR}/i18n.qrc)

  file(WRITE ${_res_file} "<RCC>\n\t<qresource prefix=\"/i18n\">\n")
  foreach(_lang ${_qm_files})
    get_filename_component(_filename ${_lang} NAME)
    file(APPEND ${_res_file} "\t\t<file>${_filename}</file>\n")
  endforeach()
  file(APPEND ${_res_file} "\t</qresource>\n</RCC>\n")

  set(${res_file} ${_res_file} PARENT_SCOPE)
endfunction()
