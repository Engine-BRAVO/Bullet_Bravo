cmake_minimum_required(VERSION 3.10)

# Define the project name
project(BulletBravo)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Add an option to enable debug mode
option(ENABLE_DEBUG "Enable debug mode" OFF)

if(ENABLE_DEBUG)
    message(STATUS "Debug mode enabled")
    set(CMAKE_BUILD_TYPE Debug)
    add_compile_options(-g -O0)
else()
    message(STATUS "Debug mode disabled")
    set(CMAKE_BUILD_TYPE Release)
    add_compile_options(-O3)  # Optimization flag for Release build
endif()

# Disable the ENABLE_TESTGAME option for the Engine-Bravo library
set(ENABLE_TESTGAME OFF CACHE BOOL "Disable building the testGame" FORCE)

# Add engine bravo library
add_subdirectory(external/Engine-Bravo/code)

# Specify the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set the source directory
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(ENGINE_BRAVO_DIR ${PROJECT_SOURCE_DIR}/external/Engine-Bravo/code)

file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/inc/*.h)

# Find all source files in the src directory
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Automatically search for subdirectories in the include directory
file(GLOB SUBDIRS RELATIVE ${ENGINE_BRAVO_DIR}/inc ${ENGINE_BRAVO_DIR}/inc/*)

# Add the root include directory
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/inc)

# Add the Engine-Bravo include directory
target_include_directories(${PROJECT_NAME} PRIVATE ${ENGINE_BRAVO_DIR}/inc)

# Add each subdirectory under the Engine-Bravo include directory
foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${ENGINE_BRAVO_DIR}/inc/${subdir})
        target_include_directories(${PROJECT_NAME} PRIVATE ${ENGINE_BRAVO_DIR}/inc/${subdir})
    endif()
endforeach()

# Add a dependency to ensure the EngineBravo library is built before the GameProject executable
add_dependencies(${PROJECT_NAME} Engine_BRAVO)

# Link the Engine-Bravo library
target_link_libraries(${PROJECT_NAME} PRIVATE Engine_BRAVO)
