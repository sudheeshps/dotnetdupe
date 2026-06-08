cmake_minimum_required(VERSION 3.10)
project(DotNetDupe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(Include)
include_directories(DotNetDupe)

# Library sources
# Force re-configure to pick up new files (like Process.cpp)
file(GLOB LIB_SOURCES "DotNetDupe/*.cpp")

# Remove dllmain.cpp from non-Windows builds if necessary, 
# but for now we'll keep it as it's often empty or has simple entry points.
if(NOT WIN32)
    list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/DotNetDupe/dllmain.cpp")
endif()

add_library(DotNetDupe SHARED ${LIB_SOURCES})

target_compile_definitions(DotNetDupe PRIVATE DOTNETDUPE_EXPORTS)

if(WIN32)
    target_link_libraries(DotNetDupe PRIVATE shlwapi)
endif()

# Demo Application
file(GLOB DEMO_SOURCES "DotNetDupeDemo/*.cpp")
add_executable(DotNetDupeDemo ${DEMO_SOURCES})
target_link_libraries(DotNetDupeDemo PRIVATE DotNetDupe)

# Testing
enable_testing()

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# For Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES "DotNetDupeTests/*.cpp")
# Remove pch.cpp if using CMake-native PCH or if not needed
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/DotNetDupeTests/pch.cpp")

add_executable(DotNetDupeTests ${TEST_SOURCES})
target_link_libraries(DotNetDupeTests PRIVATE DotNetDupe gtest_main)

include(GoogleTest)
gtest_discover_tests(DotNetDupeTests)
