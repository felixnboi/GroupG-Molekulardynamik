# Custom CMake module for Doxygen documentation generation

# Option to enable/disable Doxygen target
option(BUILD_DOCUMENTATION "Build Doxygen documentation" ON)

if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is required to build documentation.")
    endif()

    set(DOXYGEN_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/GroupG-Molekulardynamik/src/")
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doc")

    set(DOXYGEN_CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/GroupG-Molekulardynamik/Doxyfile")

    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/GroupG-Molekulardynamik/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating Doxygen documentation"
        VERBATIM
    )

    set_target_properties(doc_doxygen PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()