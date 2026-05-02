cmake_minimum_required(VERSION 3.28)
project(StoreModule LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_executable(modules_ex2 main.cpp)

target_sources(modules_ex2 PUBLIC
    FILE_SET CXX_MODULES FILES
        storeData.ixx
        store.ixx
)