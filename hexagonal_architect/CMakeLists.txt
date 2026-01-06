cmake_minimum_required(VERSION 3.14)
project(HexagonalVendingMachine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(src)

# GoogleTest setup using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Main executable definition
add_executable(vending_machine src/main.cpp)
# Link source files to executable (excluding main which we just added)
# Since we didn't create a library for sources, we need to add them here or make library interactively.
# To keep it simple, we glob sources again or use object library.

# Simpler approach: Glob all sources
file(GLOB_RECURSE PROJ_SOURCES "src/*.cpp")
# Main is already in add_executable, so we don't need to filter it out if we just list others, 
# but GLOB_RECURSE includes main.cpp.
list(FILTER PROJ_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

target_sources(vending_machine PRIVATE ${PROJ_SOURCES})
target_include_directories(vending_machine PRIVATE src)

# Build tests
add_subdirectory(test)
