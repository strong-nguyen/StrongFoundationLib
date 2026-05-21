# CMakeList.txt : CMake project for StrongFoundationLib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("StrongFoundationLib")

# Set the global default standard to C++20
set(CMAKE_CXX_STANDARD 20)

# FORCE the compiler to fail the build if it doesn't support C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific extensions (forces strict, portable ISO C++)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source to this project's executable.
add_library(StrongFoundationLib "src/ThreadPool.h" "src/ThreadPool.cpp" "src/Stopwatch.h" "src/Stopwatch.cpp" "src/ScopeStopwatch.h"  "src/StringHelper.h" "src/StringHelper.cpp" "src/Timer.h" "src/Timer.cpp" "src/MemoryPool.h" "src/MemoryPool.cpp")

# if (CMAKE_VERSION VERSION_GREATER 3.12)
#   set_property(TARGET StrongFoundationLib PROPERTY CXX_STANDARD 20)
# endif()

option(BUILD_TESTING "Build the unit tests" ON)

if(BUILD_TESTING)
    # This tells CMake to go execute the CMakeLists.txt inside the /test folder
    add_subdirectory(test)
endif()
