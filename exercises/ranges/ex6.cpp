#include <vector>
#include <string>
#include <ranges>
#include <cassert>

namespace
{
struct User {
    std::string name;
    int age;
};
/*
GOAL:
Work with std::views::zip after seeing how it combines parallel ranges.

Sometimes data is stored in parallel vectors (a structure of arrays). Zip
combines corresponding elements without copying them into a new container.

1. Zip users and isVerified.
2. Keep only rows whose verification value is true.
3. Transform each remaining row into a summary string.

vector<bool> returns a proxy rather than bool&. Accept each zipped row as
auto&&, then use auto&& [user, verified] inside each lambda.
*/
void test_1() {
    std::vector<User> users = {
        {"Alice", 30},
        {"Bob", 22},
        {"Charlie", 25},
        {"David", 19}
    };
    std::vector<bool> isVerified = {true, false, true, true};

    // TODO: Create verifiedSummary with zip -> filter -> transform.
    // auto verifiedSummary = ...;

    /* Uncomment once the pipeline has been implemented.
    assert(std::ranges::distance(verifiedSummary) == 3);
    auto first = verifiedSummary.begin();
    assert(*first == "Alice (Age: 30) - Verified");
    assert(*std::ranges::next(first, 2) == "David (Age: 19) - Verified");
    */
}
}

void ranges_ex6()
{
    test_1();
}