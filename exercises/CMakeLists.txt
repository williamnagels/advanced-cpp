cmake_minimum_required(VERSION 3.5)
project (cpptraining)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RANGES_SOURCES 
	ranges/main.cpp 
	ranges/ex1.cpp
	ranges/ex2.cpp
	ranges/ex3.cpp
	ranges/ex4.cpp
	ranges/ex5.cpp
	ranges/ex6.cpp)
add_executable(ranges ${RANGES_SOURCES})
target_compile_options(ranges PRIVATE -Wall -Wextra -Wpedantic)

set(STRONG_TYPES_SOURCES
	strongtypes/main.cpp 
	strongtypes/ex1.cpp
	strongtypes/ex2.cpp
	strongtypes/ex3.cpp
	strongtypes/ex4.cpp)
add_executable(strongtypes ${STRONG_TYPES_SOURCES})
target_compile_options(ranges PRIVATE -Wall -Wextra -Wpedantic)


set(COROUTINES_SOURCES 
	coroutines/main.cpp 
	coroutines/ex1.cpp)
add_executable(coroutines ${COROUTINES_SOURCES})
target_compile_options(coroutines PRIVATE -Wall -Wextra -Wpedantic)

