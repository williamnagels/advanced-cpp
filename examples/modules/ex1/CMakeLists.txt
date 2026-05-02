cmake_minimum_required(VERSION 3.28)
project(ModuleTest LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON) # Required for GCC module scanning

add_library(MathModule)

target_sources(MathModule
  PUBLIC
    FILE_SET CXX_MODULES FILES
      math/math.cppm
      math/add.cppm
      math/sub.cppm
)

add_executable(app main.cpp usesadd.cpp usessub.cpp)
target_link_libraries(app MathModule)