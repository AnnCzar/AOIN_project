cmake_minimum_required(VERSION 3.13)
project(ChristmasPackingCpp VERSION 1.0 LANGUAGES CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Komunikaty
message(STATUS "=== Christmas Packing Project ===")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")


# ========================================
# Pliki źródłowe (wszystkie w src/)
# ========================================
set(SOURCES
    src/ChristmasTree.cpp
    src/Geometry.cpp
    src/GreedyPacker.cpp
    src/main.cpp
)

# ========================================
# Executable
# ========================================
add_executable(christmas_greedy ${SOURCES})

# Include directories
target_include_directories(christmas_greedy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Geos
set(GEOS_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/geos-3.14.1)
set(GEOS_BUILD_DIR ${GEOS_BASE_DIR}/build)

find_path(GEOS_INCLUDE_DIR
    NAMES geos/geom/Geometry.h # A common header file in GEOS
    PATHS 
        ${GEOS_BUILD_DIR}/include 
        ${GEOS_BUILD_DIR}
    NO_DEFAULT_PATH # Only search the specified path
)

find_library(GEOS_LIBRARY
    NAMES geos geos_c 
    PATHS 
        ${GEOS_BUILD_DIR}/lib 
        ${GEOS_BUILD_DIR}/lib64
    NO_DEFAULT_PATH
)

if(GEOS_INCLUDE_DIR AND GEOS_LIBRARY)
    message(STATUS "Found GEOS Include: ${GEOS_INCLUDE_DIR}")
    message(STATUS "Found GEOS Library: ${GEOS_LIBRARY}")

    # Add the GEOS headers to the executable's include paths
    target_include_directories(christmas_greedy PRIVATE
        ${GEOS_INCLUDE_DIR}
    )

    # Link the GEOS library to the executable
    target_link_libraries(christmas_greedy PRIVATE
        ${GEOS_LIBRARY}
    )
else()
    message(FATAL_ERROR "GEOS library or headers not found! Please check the path: ${GEOS_BASE_DIR}")
endif()

# Optymalizacje
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(christmas_greedy PRIVATE -O3)
endif()

message(STATUS "Output: ${CMAKE_BINARY_DIR}/christmas_greedy")
message(STATUS "=================================")
