
#include <chrono>
#include <vector>
#include <numeric>
#include <iostream>
#include <cassert>
using namespace std::chrono_literals;
namespace
{
// GOAL: Get familiar with the std::chrono libraries of the standard library.
// There is plenty of documentation on https://en.cppreference.com/w/cpp/chrono
// which you have to use to get through this and following excercises.
void test_1()
{
  
  // TODO: The code below works but lacks type safety. There's no guarantee that the precision of the integers is consistent. 
  // Replace `int64_t` with a new type `Duration60kHz` to ensure uniform precision.
  // Duration60kHz represents a duration of 1/60000 of a second.
  // using Duration60kHz = std::chrono::duration<...>;
  int64_t time2sec = 2 * 60000; // create Duration60kHz with literal
  int64_t time100ms = 100 * 60; // create Duration60kHz with literal
  int count = 10;
  int64_t total = (time2sec + time100ms) * count;
  assert(total == 1260000);
}

void test_2()
{
  // TODO: Implement a function 'now' that returns the current UTC time as a 60kHz time point.
  // The standard library's now() function does not provide 60kHz precision, so a custom now() implementation is needed.
  auto start = std::chrono::system_clock::now();
  
  // Do some works. ignore this. trying to burn some CPU cycles. No changes needed here.
  std::vector<int> v;
  for (int i = 0; i < 10000; ++i) v.push_back(i);
  auto total = std::accumulate(v.begin(), v.end(), 0u);
  
  // TODO: Update this system_clock::now() call aswell.
  auto end = std::chrono::system_clock::now();
  auto diff = end - start;
  std::cout << "test_2 diff: " << diff.count() << " total=" << total <<std::endl;
}

// GOAL: std::chrono can prevent common math mistakes. This excercise shows how
// with strong types (like std::chrono) the compiler can flag erroneous math.
void test_3()
{
  // TODO: Change the type of the contained elements in container lap_times.
  // lap_times contains durations in milliseconds. There is a type in the standard that expresses exactly that.
  std::vector<uint64_t> lap_times_in_millis{400, 356, 402, 397, 402, 386};
  // TODO: Change the type of total_sec to the total duration in seconds
  // the only rounding to seconds precision should happen on the end result
  // Q: can you deduce the type of total_sec based on the type of elements in lap_times without explicitly specifying the duration type ?
  uint64_t total_sec = 0;
  for (auto i : lap_times_in_millis)
    //TODO: Fix the math
    total_sec += i / 1000;
  //TODO: When the TODOs above are fixed, the print should be correct.
  std::cout << "test_3: The total time is " << total_sec << " seconds\n";
  //TODO: Did you encounter any compiler warning that helped to detection loss of precision ?
}

}
 
void type_safe_ex2()
{
  test_1();
  test_2();
  test_3();
}
