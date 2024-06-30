# Generates the header files for the JavaScript enums transpiled from altv-types
if(NOT BINDINGS_SCOPE)
    set(BINDINGS_SCOPE "SHARED")
endif()

if (CMAKE_HOST_WIN32)
  add_custom_target(js-enums
    call generate-enums.bat ${BINDINGS_SCOPE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
else()
  add_custom_target(js-enums
    bash generate-enums.sh ${BINDINGS_SCOPE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
endif()
