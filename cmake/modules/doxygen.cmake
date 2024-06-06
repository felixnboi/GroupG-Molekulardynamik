# Custom CMake module for Doxygen documentation generation

# Option to enable/disable Doxygen target
option(BUILD_DOCUMENTATION "Build Doxygen documentation" ON)

if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is required to build documentation.")
    endif()

    # # Set the input files for Doxygen
    # set(DOXYGEN_INPUT_FILES
    #     "${CMAKE_SOURCE_DIR}/src/MolSim.cpp"
    #     "${CMAKE_SOURCE_DIR}/src/Particle.h"
    #     "${CMAKE_SOURCE_DIR}/src/ParticleContainer.h"
    # )

    # Set the output directory for Doxygen
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doxys_documentation")

    # Configure Doxygen file directly
    set(DOXYGEN_CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/GroupG-Molekulardynamik/Doxyfile")

    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    set_target_properties(doc_doxygen PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()