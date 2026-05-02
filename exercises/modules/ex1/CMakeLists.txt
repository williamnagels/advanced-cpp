cmake_minimum_required(VERSION 3.28)
project(LegacyMathLibrary LANGUAGES CXX)

# Use C++20 just to keep the compiler settings consistent, 
# though this code works on C++11.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the executable and its two source files
add_executable(modules_ex1 
    main.cpp 
    mathLib.cpp
)