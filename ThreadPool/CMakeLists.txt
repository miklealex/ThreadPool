cmake_minimum_required(VERSION 3.20)

# Project information
project(ThreadPool VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the library as an interface target
add_library(ThreadPool INTERFACE)

# Specify include directories for the library
target_include_directories(ThreadPool INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Add tests if BUILD_TESTING is enabled
include(CTest) # Enables the BUILD_TESTING option
if(BUILD_TESTING)
    enable_testing()
    find_package(GTest CONFIG REQUIRED COMPONENTS GTest GMock)
    # Add a simple test executable
    add_executable(ThreadPoolTests tests/main.cpp)

    # Link the library to the test executable
    target_link_libraries(ThreadPoolTests PRIVATE ThreadPool gtest gmock)

    # Register the test with CTest
    add_test(NAME ThreadPoolTests COMMAND ThreadPoolTests)
endif()

