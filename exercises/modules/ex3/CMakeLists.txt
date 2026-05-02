# File does not need need changes.

cmake_minimum_required(VERSION 3.28)
project(ModernWrapper LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
add_executable(modules_ex3 main.cpp)
target_sources(modules_ex3 PUBLIC
    FILE_SET CXX_MODULES FILES
        legacyLibWrapper.ixx
)