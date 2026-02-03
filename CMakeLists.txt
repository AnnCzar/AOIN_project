cmake_minimum_required(VERSION 3.13)
project(ChristmasPackingCpp VERSION 1.0 LANGUAGES CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Komunikaty
message(STATUS "=== Christmas Packing Project ===")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Optymalizacja Ania---------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "Build type not specified, defaulting to Release")
endif()

# Dla MSVC (Visual Studio)
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /Oi /Ot /GL /DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG /OPT:REF /OPT:ICF")
    message(STATUS "Using MSVC optimizations: /O2 /GL /LTCG")
    
# Dla MinGW/g++ na Windows
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")
    message(STATUS "Using GCC/Clang optimizations: -O3 -march=native -flto")
endif()

# Optymalizacja Ania koniec---------------




# ========================================
# Pliki źródłowe (wszystkie w src/)
# ========================================
set(SOURCES
    # src/test.cpp 
    src/ChristmasTree.cpp
    src/GreedyPacker.cpp
    src/main.cpp
    src/CSVWriter.cpp
    src/evaluation.cpp
    src/submission_file.cpp
    src/submission_file_greedy.cpp
    src/simulated_annealing.cpp
    src/operators.cpp
)

# ========================================
# Executable
# ========================================
add_executable(christmas_greedy1                                              ${SOURCES})

# Include directories
target_include_directories(christmas_greedy1                                               PRIVATE
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
    target_include_directories(christmas_greedy1                                               PRIVATE
        ${GEOS_INCLUDE_DIR}
    )

    # Link the GEOS library to the executable
    target_link_libraries(christmas_greedy1                                               PRIVATE
        ${GEOS_LIBRARY}
    )
else()
    message(FATAL_ERROR "GEOS library or headers not found! Please check the path: ${GEOS_BASE_DIR}")
endif()



    # # # GEOS - ANIA
    # set(GEOS_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/geos-3.14.1)
    # set(GEOS_INSTALL_DIR ${GEOS_BASE_DIR}/install)  # katalog instalacji
    
    # set(GEOS_INCLUDE_DIR ${GEOS_INSTALL_DIR}/include)
    # set(GEOS_LIB_DIR ${GEOS_INSTALL_DIR}/lib)

    # find_library(GEOS_LIBRARY
    #     NAMES geos
    #     PATHS ${GEOS_LIB_DIR}
    #     NO_DEFAULT_PATH
    # )
    
    # if(NOT GEOS_LIBRARY)
    #     find_library(GEOS_LIBRARY
    #         NAMES geos_c
    #         PATHS ${GEOS_LIB_DIR}
    #         NO_DEFAULT_PATH
    #     )
    # endif()
    

    # if(EXISTS ${GEOS_INCLUDE_DIR} AND GEOS_LIBRARY)
    #     message(STATUS "  Found GEOS (manual configuration)")
    #     message(STATUS "  Include: ${GEOS_INCLUDE_DIR}")
    #     message(STATUS "  Library: ${GEOS_LIBRARY}")
        
    #     target_include_directories(christmas_greedy1                                               PRIVATE ${GEOS_INCLUDE_DIR})
    #     target_link_libraries(christmas_greedy1                                               PRIVATE ${GEOS_LIBRARY})
    # else()
    #     message(FATAL_ERROR "  GEOS not found!")
    #     message(FATAL_ERROR "   Expected include dir: ${GEOS_INCLUDE_DIR}")
    #     message(FATAL_ERROR "   Expected library dir: ${GEOS_LIB_DIR}")
    #     message(FATAL_ERROR "   Install GEOS first: cd ${GEOS_BASE_DIR} && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . && cmake --install . --prefix ../install")
    # endif()

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(christmas_greedy1                                 PUBLIC OpenMP::OpenMP_CXX)
endif()


# Optymalizacje
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(christmas_greedy1                      PRIVATE -O3)
endif()


#  Optymalizacja Ania------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        # Dodatkowe optymalizacje MSVC
        target_compile_options(christmas_greedy1                  PRIVATE
            /fp:fast      # Szybka arytmetyka zmiennoprzecinkowa
            /GS-          # Wyłącz buffer security check
        )
    else()
        # Dodatkowe optymalizacje GCC/Clang
        target_compile_options(christmas_greedy1                 PRIVATE
            -ffast-math         # Szybsza matematyka
            -funroll-loops      # Rozwijanie pętli
            -fomit-frame-pointer
        )
    endif()
    
    message(STATUS "Release optimizations enabled")
endif()
#  Optymalizacja Ania - koniec------------

message(STATUS "Output: ${CMAKE_BINARY_DIR}/christmas_greedy1                                       ")
message(STATUS "=================================")
