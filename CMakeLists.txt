cmake_minimum_required(VERSION 3.25)

project(
    STL-Fix
    VERSION 0.1.0
    LANGUAGES CXX
)

# Core -> independent of Qt/GUI
# Core -> Is not aware and should not be of GUI existing.

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(STLFIX_BUILD_TESTS "Build STLFix tests" ON)

add_subdirectory(src/core)
add_subdirectory(src/cli)

if(STLFIX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()