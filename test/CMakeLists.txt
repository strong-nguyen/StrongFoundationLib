cmake_minimum_required(VERSION 3.14)

# 1. Ask CMake to find the gtest package installed by vcpkg
find_package(GTest CONFIG REQUIRED)

file(GLOB TEST_SOURCES "*_test.cpp")

message(STATUS "--- MY TEST SOURCES ARE: ${TEST_SOURCES} ---")

add_executable(foundation_unit_tests ${TEST_SOURCES}  "Stopwatch_test.cpp")

# 2. Link against the targets provided by the find_package call
target_link_libraries(foundation_unit_tests 
    PRIVATE 
        GTest::gtest_main  # Provided natively by vcpkg's gtest package
        StrongFoundationLib
)

# Allow cmake to discover test cases
include(GoogleTest)
gtest_discover_tests(foundation_unit_tests
    DISCOVERY_TIMEOUT 60
    PROPERTIES DISCOVERY_MODE PRE_TEST)