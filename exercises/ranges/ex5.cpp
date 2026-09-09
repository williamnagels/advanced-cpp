#include <vector>
#include <string>
#include <ranges>
#include <iostream>
#include <tuple>

namespace
{
struct User 
{
    std::string name;
    int age;
};
/*
GOAL:
The goal of this exercise is to work with a zip view 'std::views::zip'

Sometimes data is stored in "parallel vectors" rather than a single struct. 
e.g. structure of arrays.
Zip allows you to stitch these back together temporarily for processing.

1. Zip the Ranges: Combine 'users' and 'isVerified' into a single range.
2. Filter: Keep only the pairs where the verification bool is true.
3. Transform: Create a string summary for each verified user. 
   - Use C++17 Structured Bindings [u, v] inside your lambdas 
     to unpack the tuple produced by the zip view. 

Note: Structured bindings make it much clearer which part of the tuple 
is the 'User' and which is the 'bool' compared to using std::get.
*/
void test_1() {
    std::vector<User> users = {
        {"Alice", 30},
        {"Bob", 22},
        {"Charlie", 25},
        {"David", 19}
    };

    std::vector<bool> isVerified = { true, false, true, true };
    
    // TODO: Create a view pipeline using zip, filter, and transform.
    // Use structured bindings in the lambdas: [](const auto& pair) { auto& [user, verified] = pair; ... }
    // auto verifiedSummary = ...

    /* Uncomment these asserts when the pipeline has been implemented
    // 1. Check the count (Bob should be filtered out)
    auto resultCount = std::ranges::distance(verifiedSummary);
    assert(resultCount == 3);

    // 2. Check the first element
    auto it = verifiedSummary.begin();
    assert(*it == "Alice (Age: 30) - Verified");

    // 3. Check the last element (using ranges::next to skip Charlie)
    auto last = std::ranges::next(it, 2); 
    assert(*last == "David (Age: 19) - Verified");
    */

}
}
void ranges_ex3()
{
    test_1();
}