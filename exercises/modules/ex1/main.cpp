#include "mathLib.h"
#include <iostream>
/*
GOAL:
1. Remove mathLib includes. keep iostream for testing purposes. 
   you can try removing iostream but you will have to build the iostream module in that case.
   If you got some spare time you can try building the iostream module and then removing the include here and importing it instead.
2. You will need to replace the mathLib.cpp and Mathlib.h file with modules
   write 1 Module.cppm file that contains the implementation of add and subtract, and exports them.
   Dont worry about partitioning the module for this exercise, we will do that in the next one.
3. Update the CMakeLists.txt to build the module and link it to the main app. There are examples in the slides
4. Test that the main app can call the add and subtract functions from the module and print the results. 
you can copy paste from, in case you are stuck.

FYI:
I build this using
the container has a recent enough Ninja and g++ version.

docker run --rm -v $ROOT_CPPTRAINING/exercises/modules/ex1:/source -v build:/build cpptraining /bin/sh -c "ls -alrt /build; cmake -G Ninja -B /build -S /source; cmake --build /build; build/modules_ex1"

To read as: mounted sources from exercises/modules/ex1 to /source in the container, 
and build output to /build. 
Then we run cmake to configure and build the project, and finally execute the built app modules_ex1.
*/
int main() {
    std::cout << "Addition: " << MathLib::add(10, 5) << "\n";
    std::cout << "Subtraction: " << MathLib::subtract(10, 5) << "\n";
    return 0;
}