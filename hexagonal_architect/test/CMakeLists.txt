# Discover all test files
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# Discover all source files except main.cpp
file(GLOB_RECURSE SRC_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(FILTER SRC_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Create test executable
add_executable(unit_tests ${TEST_SOURCES} ${SRC_SOURCES})

# Link against GoogleTest and GoogleMock
target_link_libraries(unit_tests GTest::gtest_main GTest::gmock_main)

# Integarte with CTest
include(GoogleTest)
gtest_discover_tests(unit_tests)
