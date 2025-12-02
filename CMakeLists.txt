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


# Optymalizacje
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(christmas_greedy PRIVATE -O3)
endif()

message(STATUS "Output: ${CMAKE_BINARY_DIR}/christmas_greedy")
message(STATUS "=================================")
