cmake_minimum_required(VERSION 3.16)

project(Bomberman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Threads (pthreads en Linux)
find_package(Threads REQUIRED)

# Headers
include_directories(include)

# Sources
file(GLOB_RECURSE SRC_FILES src/*.cpp)
list(REMOVE_ITEM SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/main.cpp)

# Library
add_library(core ${SRC_FILES})
target_include_directories(core PUBLIC include)

# Main app
add_executable(bomberman src/core/main.cpp)
target_link_libraries(bomberman core sfml-graphics sfml-window sfml-system Threads::Threads)

# Test
add_executable(PlayerTest tests/PlayerTest.cpp)
target_link_libraries(PlayerTest core)

# Test
add_executable(TableroTest tests/TableroTest.cpp)
target_link_libraries(TableroTest core)

# Test
add_executable(RenderSnapshotTest tests/RenderSnapshotTest.cpp)
target_link_libraries(RenderSnapshotTest core sfml-graphics sfml-window sfml-system Threads::Threads)

# Flags
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(bomberman PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})