cmake_minimum_required(VERSION 3.20)

# Project information
project(CPP_Utils VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the library as an interface target
add_library(CPP_Utils INTERFACE)

# Specify include directories for the library
target_include_directories(CPP_Utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_subdirectory(ThreadPool)

