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

# Ejecutable
add_executable(bomberman ${SRC_FILES})

# Linkeo
target_link_libraries(bomberman
    sfml-graphics
    sfml-window
    sfml-system
    Threads::Threads
)

# Flags
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(bomberman PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})