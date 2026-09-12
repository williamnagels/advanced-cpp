cmake_minimum_required(VERSION 3.20)
project(ranges_pipeline_benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(benchmark CONFIG REQUIRED)

add_library(ranges_pipeline STATIC pipeline.cpp)
target_include_directories(ranges_pipeline PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(ranges_pipeline PRIVATE -Wall -Wextra -Wpedantic)

add_executable(ranges_pipeline_benchmark benchmark.cpp)
target_link_libraries(ranges_pipeline_benchmark
    PRIVATE ranges_pipeline benchmark::benchmark)
target_compile_options(ranges_pipeline_benchmark
    PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
add_executable(ranges_pipeline_test pipeline_test.cpp)
target_link_libraries(ranges_pipeline_test PRIVATE ranges_pipeline)
target_compile_options(ranges_pipeline_test PRIVATE -Wall -Wextra -Wpedantic)
add_test(NAME ranges_pipeline_test COMMAND ranges_pipeline_test)