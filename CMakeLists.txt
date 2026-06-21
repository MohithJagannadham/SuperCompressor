cmake_minimum_required(VERSION 3.14)
project(SuperCompressor VERSION 3.0 DESCRIPTION "Enterprise Modular Layered Architecture Compressor")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Core library source tracking paths
set(CORE_SOURCES 
    BitStream.cpp
    HuffmanCompressor.cpp 
    LZWCompressor.cpp
)

# 1. Main Application Utility Engine Build
add_executable(compressor main.cpp ${CORE_SOURCES})
target_compile_options(compressor PRIVATE -Wall -Wextra -Wpedantic)

# 2. Zero-Dependency Native Test Suite
add_executable(run_tests tests.cpp ${CORE_SOURCES})