cmake_minimum_required(VERSION 3.22)

# ==============================================================================
# CONTAINER-ONLY BUILD CONFIGURATION
# ==============================================================================
# This project builds ONLY in Docker containers, never locally.
# All paths are container paths: /opt/arm-toolchain/, /usr/bin/, etc.

message(STATUS "✓ Building in Docker container")
#set(CMAKE_TOOLCHAIN_FILE "${sourceDir}/cmake/gcc-arm-none-eabi.cmake")
# ==============================================================================
# CONTAINER TOOLCHAIN PATHS (Fixed and Explicit)
# ==============================================================================
set(CONTAINER_TOOLCHAIN_PATH "/opt/arm-toolchain/gcc")


set(CONTAINER_OPENOCD_PATH "/opt/openocd")
set(CONTAINER_PYTHON_PATH "/usr/bin/python3")
set(CONTAINER_DOXYGEN_PATH "/usr/bin/doxygen")
set(CONTAINER_CPPCHECK_PATH "/usr/bin/cppcheck")



# Set Doxygen executable (container path)
set(DOXYGEN_EXECUTABLE "${CONTAINER_DOXYGEN_PATH}")

# Set CPPCheck executable (container path)
set(CMAKE_C_CPPCHECK "${CONTAINER_CPPCHECK_PATH}")

# Set Python executable (container path)
set(Python3_EXECUTABLE "${CONTAINER_PYTHON_PATH}")

# Output directories in container
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY"/build/output")
set(DOXYGEN_OUTPUT_DIRECTORY "/build/documentation")

message(STATUS "")
message(STATUS "=== Container Configuration ===")
message(STATUS "C Compiler:       ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler:     ${CMAKE_CXX_COMPILER}")
message(STATUS "Toolchain:        ${CONTAINER_TOOLCHAIN_PATH}")
message(STATUS "Doxygen:          ${DOXYGEN_EXECUTABLE}")
message(STATUS "CPPCheck:         ${CONTAINER_CPPCHECK_PATH}")
message(STATUS "Output directory: /build/")
message(STATUS "")

# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME RTOSP_STM32L412RB)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Enable testing infrastructure (CTest)
enable_testing()

# Option to build ONLY unit tests (skip application and HAL drivers)
option(BUILD_TESTING_ONLY "Build ONLY unit tests, skip application build" OFF)

if(NOT BUILD_TESTING_ONLY)
    # ========================================================================
    # APPLICATION BUILD (ARM Embedded)
    # ========================================================================
    
    # Create an executable object type
    add_executable(${CMAKE_PROJECT_NAME})

    # Add STM32CubeMX generated sources, drivers, and middleware
    ####. PLEASE UPDATE ANY CHANGES DONE TO PROJECT STRUCTURE IN THE CMAKE FILES TO AVOID BUILD ISSUES. ####    

    add_subdirectory(cmake/stm32cubemx)

    # Link directories setup
    target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined library search paths
    )

    # Add sources to executable
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user sources here
    )

    # Add include paths
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined include paths
    )

    # Add project symbols (macros)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined symbols
    )

    # Remove wrong libob.a library dependency when using cpp files
    list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

    # Add linked libraries
    target_link_libraries(${CMAKE_PROJECT_NAME}
        stm32cubemx

        # Add user defined libraries
    )


    # Convert to bin file
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}.bin
        COMMENT "Generating binary file: ${CMAKE_PROJECT_NAME}.bin"
    )

    # Also generate hex file (optional, useful for flashing)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}.hex
        COMMENT "Generating hex file: ${CMAKE_PROJECT_NAME}.hex"
    )

    set(CMAKE_C_CPPCHECK
        "cppcheck;--addon=misra;--enable=all;--inconclusive;--std=c11;--force;--inline-suppr;--suppress=missingIncludeSystem;--suppress=misra-config"
    )
else()
    # ========================================================================
    # UNIT TEST BUILD (Native Host Compilation)
    # ========================================================================
    message(STATUS "Building UNIT TESTS ONLY (skipping application)")
endif()

# Add unit tests (optional, can be disabled with -DBUILD_TESTING=OFF)
option(BUILD_TESTING "Build unit tests" ON)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# ========================================================================
# DOCUMENTATION TARGET
# ========================================================================
# Add custom target for building Doxygen documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )
else()
    message(WARNING "Doxygen not found. Documentation target will not be available. Install Doxygen to enable documentation generation.")
endif()

# ========================================================================
# CODE COMPLEXITY ANALYSIS TARGET (Lizard)
# ========================================================================
find_program(LIZARD_EXECUTABLE lizard)

if(LIZARD_EXECUTABLE)
    # Create output directory for complexity reports
    set(COMPLEXITY_REPORT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/documentation/report")
    
    add_custom_target(complexity
        COMMAND ${CMAKE_COMMAND} -E make_directory ${COMPLEXITY_REPORT_DIR}
        COMMAND ${LIZARD_EXECUTABLE} 
            Core/Src/main.c 
            -l c 
            -w 
            --thresholds nloc=50,ccn=15
            -o ${COMPLEXITY_REPORT_DIR}/complexity_report.log
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_cc_report.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating code complexity analysis with Lizard..."
        VERBATIM
    )
    
    # Combined target: Docs + Complexity
    add_custom_target(docs-full
        COMMAND cmake --build ${CMAKE_BINARY_DIR} --target docs
        COMMAND cmake --build ${CMAKE_BINARY_DIR} --target complexity
        COMMENT "Generating full documentation with Doxygen and complexity analysis..."
        DEPENDS docs complexity
    )
else()
    message(WARNING "Lizard not found. Complexity analysis target unavailable. Install: pip3 install lizard")
endif()




