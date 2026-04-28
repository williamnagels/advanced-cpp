project(compilertest LANGUAGES CXX)
add_executable(compilertest main.cpp)
target_compile_features(compilertest PRIVATE cxx_std_23)
set_target_properties(compilertest PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)