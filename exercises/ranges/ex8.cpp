#include <vector>
#include <string>
#include <ranges>
#include <iostream>
#include <cassert>

namespace
{
template<std::ranges::view V, typename Pred>
class trim_view : public std::ranges::view_interface<trim_view<V, Pred>> {
    // TODO: Store the base view and the predicate
    // TODO: Implement begin() and end()
};

// TODO: Implement the closure to support the | operator
struct trim_fn : std::ranges::range_adaptor_closure<trim_fn> {
    // Hint: Needs to handle both (range, pred) and (pred) for piping
};

inline constexpr trim_fn trim;

/*
GOAL:
The goal of this exercise is to implement a custom 'trim_view' and its 
corresponding Pipe Adapter.

Standard library views like 'drop_while' only work from the front. A 'trim' 
view should lazily ignore elements at the beginning AND the end that 
match a specific predicate.

1. The View: Create 'trim_view' inheriting from 'std::ranges::view_interface'.
   It should wrap a range and a predicate.
2. The Logic: Your 'begin()' should find the first element NOT matching 
   the predicate. Your 'end()' should find the last element NOT matching 
   the predicate
3. The Adapter: Inherit from 'std::ranges::range_adaptor_closure' to allow 
   the syntax: 'range | trim(predicate)'.
*/
void test_1() {
    std::vector<int> data = {0, 0, 7, 8, 9, 0, 0};
    auto is_zero = [](int i) { return i == 0; };

    // TODO: uncomment when trim has been implemented
    //auto trimmed = data | trim(is_zero);

    /*
    TODO: uncommment these asserts when the above pipeline has been implemented
    assert(std::ranges::distance(trimmed) == 3);
    assert(*trimmed.begin() == 7);
    assert(*std::ranges::prev(trimmed.end()) == 9);
    */
}
}
void ranges_ex6()
{
    test_1();
}
