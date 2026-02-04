cmake_minimum_required(VERSION 3.13)
project(ChristmasPackingCpp VERSION 1.0 LANGUAGES CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ania ---------------------

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "Build type not specified, defaulting to Release")
endif()


# Dla MSVC (Visual Studio)
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /Oi /Ot /GL /Zi /DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG /OPT:REF /OPT:ICF /DEBUG /PROFILE")
    message(STATUS "Using MSVC optimizations: /O2 /GL /LTCG")
    
# Dla MinGW/g++ na Windows
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")
    message(STATUS "Using GCC/Clang optimizations: -O3 -march=native -flto")
endif()


# Ania koniec-----------------------



# Komunikaty
message(STATUS "=== Christmas Packing Project ===")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")






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
    src/DE.cpp 
    src/Pso.cpp
    src/PsoParticle.cpp
    src/Tabu.cpp
)

# ========================================
# Executable
# ========================================
add_executable(christmass_ts                                              ${SOURCES})

# Include directories
target_include_directories(christmass_ts                                               PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)



# Geos
#set(GEOS_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/geos-3.14.1)
#set(GEOS_BUILD_DIR ${GEOS_BASE_DIR}/build)
#
#
#find_path(GEOS_INCLUDE_DIR
#    NAMES geos/geom/Geometry.h # A common header file in GEOS
#    PATHS 
#        ${GEOS_BUILD_DIR}/include 
#        ${GEOS_BUILD_DIR}
#    NO_DEFAULT_PATH # Only search the specified path
#)
#
#find_library(GEOS_LIBRARY
#    NAMES geos geos_c 
#    PATHS 
#        ${GEOS_BUILD_DIR}/lib 
#        ${GEOS_BUILD_DIR}/lib64
#    NO_DEFAULT_PATH
#)
#
#if(GEOS_INCLUDE_DIR AND GEOS_LIBRARY)
#    message(STATUS "Found GEOS Include: ${GEOS_INCLUDE_DIR}")
#    message(STATUS "Found GEOS Library: ${GEOS_LIBRARY}")
#
#    # Add the GEOS headers to the executable's include paths
#    target_include_directories(christmass_ts                                               PRIVATE
#        ${GEOS_INCLUDE_DIR}
#    )
#
#    # Link the GEOS library to the executable
#    target_link_libraries(christmass_ts                                               PRIVATE
#        ${GEOS_LIBRARY}
#    )
#else()
#    message(FATAL_ERROR "GEOS library or headers not found! Please check the path: ${GEOS_BASE_DIR}")
#endif()




# GEOS ---------------- ANIA
    set(GEOS_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/geos-3.14.1)
    set(GEOS_INSTALL_DIR ${GEOS_BASE_DIR}/install)  # katalog instalacji

    set(GEOS_INCLUDE_DIR ${GEOS_INSTALL_DIR}/include)
    set(GEOS_LIB_DIR ${GEOS_INSTALL_DIR}/lib)

    find_library(GEOS_LIBRARY
        NAMES geos
        PATHS ${GEOS_LIB_DIR}
        NO_DEFAULT_PATH

    )

    if(NOT GEOS_LIBRARY)
        find_library(GEOS_LIBRARY
            NAMES geos_c
            PATHS ${GEOS_LIB_DIR}
            NO_DEFAULT_PATH
        )
    endif()


    if(EXISTS ${GEOS_INCLUDE_DIR} AND GEOS_LIBRARY)
        message(STATUS "  Found GEOS (manual configuration)")
        message(STATUS "  Include: ${GEOS_INCLUDE_DIR}")
        message(STATUS "  Library: ${GEOS_LIBRARY}")

        target_include_directories(christmass_ts PRIVATE ${GEOS_INCLUDE_DIR})
        target_link_libraries(christmass_ts PRIVATE ${GEOS_LIBRARY})
        # Automatyczne kopiowanie DLL po zbudowaniu
        add_custom_command(TARGET christmass_ts POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/libs/geos-3.14.1/install/bin/geos.dll"
            "$<TARGET_FILE_DIR:christmass_ts>"
            COMMENT "Kopiowanie geos.dll"
        )

        add_custom_command(TARGET christmass_ts POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/libs/geos-3.14.1/install/bin/geos_c.dll"
            "$<TARGET_FILE_DIR:christmass_ts>"
            COMMENT "Kopiowanie geos_c.dll"
)

    else()
        message(FATAL_ERROR "  GEOS not found!")
        message(FATAL_ERROR "   Expected include dir: ${GEOS_INCLUDE_DIR}")
        message(FATAL_ERROR "   Expected library dir: ${GEOS_LIB_DIR}")
        message(FATAL_ERROR "   Install GEOS first: cd ${GEOS_BASE_DIR} && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . && cmake --install . --prefix ../install")
    endif()
# GEOS ----------------  koniec  ANIA




find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(christmass_ts                                 PUBLIC OpenMP::OpenMP_CXX)
endif()




#  Optymalizacja Ania------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        # Dodatkowe optymalizacje MSVC
        target_compile_options(christmass_ts PRIVATE
            /fp:fast      # Szybka arytmetyka zmiennoprzecinkowa
            /GS-          # Wyłącz buffer security check
            /Gy 
        )
    else()
        # Dodatkowe optymalizacje GCC/Clang
        target_compile_options(christmass_ts PRIVATE
            -ffast-math         # Szybsza matematyka
            -funroll-loops      # Rozwijanie pętli
            -fomit-frame-pointer
            -finline-functions 
        )
    endif()
    
    message(STATUS "Release optimizations enabled")
endif()
#  Optymalizacja Ania - koniec------------

message(STATUS "Output: ${CMAKE_BINARY_DIR}/christmass_ts                                       ")
message(STATUS "=================================")
