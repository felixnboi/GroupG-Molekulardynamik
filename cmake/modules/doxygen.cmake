# Custom CMake module for Doxygen documentation generation

# Option to enable/disable Doxygen target
option(BUILD_DOCUMENTATION "Build Doxygen documentation" ON)

if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is required to build documentation.")
    endif()

    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    set_target_properties(doc_doxygen PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()