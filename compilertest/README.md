# Compiler test
## Building
This CMake project verifies if your build environment is capable of building all exercises. The key requirements are your compiler's support for C++23 features and CMake.

If your current compiler doesn't meet these requirements, or if you're using an older version of CMake/Clang/GCC and don't want to modify your system, there’s an Alpine-based Docker container provided
that includes the correct GCC version and CMake in docker/. The container is also a handy solution if you're using Windows or need a clean, isolated environment for building.
If you use the container, its best to volume mount the compilertest sources using the -v start-up parameter.
The example below assumes that **<TRAINING_ROOT>** is replaced with an actual path on your system. It should be replaced with the path to the advanced-cpp training root folder.
- Build the container
```
docker build -t cpptraining <TRAINING_ROOT>/docker
```
- Build and run the test binary in the container
```
docker run --rm -v <TRAINING_ROOT>/compilertest:/source cpptraining /bin/sh -c "cmake -B /build -S /source ; cmake --build /build; /build/compilertest"
```
If you are not using a container you can build the compilertest binary using the commands from the second code snippet. The paths '/build' and '/source' may have to be updated to something
that works on your system.
## Expected result
Expected output when running the compilerTest binary:
```
Testing C++23 Features...
Success: C++23 std::expected is available!
Ranges test (even numbers): 2 4
```
There should not be any build errors.
