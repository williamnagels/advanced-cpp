# Exercises

## Build status

![Build Status](https://github.com/williamnagels/advanced-cpp/actions/workflows/dockerbuild.yaml/badge.svg)

The GitHub workflow* builds the Docker container located in docker/. It then uses the containerized compiler to build both the CMake project in the exercises folder (this folder) and the compilertest project within the extra/compilertest directory.

*advanced-cpp/.github/workflows/build-in-docker.yaml
  
## Native Compile the test binaries:
The instructions in this README file assume that a variable has been defined that stores the C++ Training clone destination folder, as shown below:
```
ROOT_CPPTRAINING=/home/william/cpptraining
```
### Generate Makefiles
```
cmake -B $ROOT_CPPTRAINING/build/ -S $ROOT_CPPTRAINING/exercises/
```
In this example the makefiles are generated in the ***build*** directory /home/william/cpptraining/build (-B). The CMakefile used to generated this makefile can be found in **source** directory /home/william/cpptraining/exercises/ (-S)
This step should never fail on a clean clone. The build folder must not exist, CMake will create it for you if it has sufficient permissions to create a folder.
### Build project using generated Makefiles
```
cmake --build $ROOT_CPPTRAINING/build
```
Alternatively, you can use make directly instead of going through CMake:
```
make -C $ROOT_CPPTRAINING/build
```
At this stage, the compiler may issue some warnings. These occur because the source files contain intentionally problematic code that has to be fixed as part of the exercises. Warnings are expected, but there should never be any compiler errors on a clean clone and the build should succeed.

### Run test binary
```
$ROOT_CPPTRAINING/strong
```
Asserts will occur with a clean checkout, but crashes or core dumps should never happen.
### Clean
CMake generates several targets in the makefile, one of which is clean:
```
cmake --build $ROOT_CPPTRAINING/build --target clean
```
This clean target removes all files that have been built in $ROOT_CPPTRAINING/build.
### CMake with Ninja
CMake can also generate build files for Ninja, if installed:
```
cmake --build $ROOT_CPPTRAINING/build -G Ninja
```
By default CMake will generate UNIX makefiles but with Ninja, it is possible to clean specific sub-projects instead of cleaning all targets at once:
```
ninja -C $ROOT_CPPTRAINING/build -t clean value
```
In this example, value represents the specific sub-project or target you wish to clean.
## Docker container
You can use all native build commands, but they should be executed inside the Docker container. The following command will create a build folder in $ROOT_CPPTRAINING. This folder will be volume-mounted to the Docker container, with the native build directory mapped to /build within the container, which serves as the build destination for CMake.
```
mkdir $ROOT_CPPTRAINING/build
docker run --rm -v $ROOT_CPPTRAINING/exercises:/source -v $ROOT_CPPTRAINING/build:/build cpptraining /bin/sh -c "cmake -B /build -S /source ; cmake --build /build"
```
## Solving the exercises
1. Navigate to the folder of the module you're interested in (e.g., containers). Open the corresponding exercise file, such as containers/ex1.cpp, to start with the first exercise.
2. Look for the symbol test_1 within that file and read the associated documentation. The instructions should clarify what needs to be done. Each test file (e.g. containers/ex1.cpp) will typically contain several local symbols like test_1, test_2, etc., and one global symbol, such as cont_ex1. This global symbol is called by the test binary. You can see the global symbols needed to build each binary in the main.cpp file in each submodule.
3. After making changes to the source files, compile the project.
4. Run the binary that contains the excercise you were making to test if your changes are correct.