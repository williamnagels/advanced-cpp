#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <functional>
#include <utility>

namespace
{
template<std::ranges::view V, typename Pred>
requires std::ranges::bidirectional_range<V> &&
         std::ranges::common_range<V> &&
         std::indirect_unary_predicate<Pred, std::ranges::iterator_t<V>>
class trim_view : public std::ranges::view_interface<trim_view<V, Pred>> {
    // TODO: Store V base_ and Pred predicate_. Consider [[no_unique_address]]
    // for the predicate so an empty lambda need not increase object size.
public:
    // TODO: Add a default constructor and a constructor taking V and Pred.

    // TODO: begin() returns the first element for which predicate_ is false.
    // Use std::ranges::find_if_not.

    // TODO: end() walks backwards from ranges::end(base_) until it reaches
    // begin() or finds an element for which predicate_ is false. Return the
    // iterator one past that element.
};

// TODO: Add a deduction guide that wraps an input range with views::all_t.

/* TODO: Implement a closure that stores a predicate and supports range | closure.
template<typename Pred>
struct trim_closure : std::ranges::range_adaptor_closure<trim_closure<Pred>> {
    [[no_unique_address]] Pred predicate_;

    template<std::ranges::viewable_range R>
    auto operator()(R&& range) const {
        // Return trim_view(std::views::all(std::forward<R>(range)), predicate_).
    }
};

struct trim_fn {
    // TODO: operator()(range, predicate) applies trim directly.
    // TODO: operator()(predicate) returns trim_closure<Pred> for piping.
};

inline constexpr trim_fn trim;
*/

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
   the predicate.
3. The Adaptor: Inherit from std::ranges::range_adaptor_closure to allow
   the syntax: 'range | trim(predicate)'.
4. Edge Cases: An empty range and a range containing only matching elements
   must both produce an empty view without decrementing before begin().
*/
void test_1() {
    std::vector<int> data = {0, 0, 7, 8, 9, 0, 0};
    auto is_zero = [](int i) { return i == 0; };

    // TODO: Uncomment once trim has been implemented.
    // auto trimmed = data | trim(is_zero);
    // auto empty = std::vector<int>{} | trim(is_zero);
    // auto allZeros = std::vector{0, 0, 0} | trim(is_zero);

    /*
    TODO: Uncomment once the pipeline has been implemented.
    assert(std::ranges::distance(trimmed) == 3);
    assert(*trimmed.begin() == 7);
    assert(*std::ranges::prev(trimmed.end()) == 9);
    assert(std::ranges::empty(empty));
    assert(std::ranges::empty(allZeros));
    */
}
}

void ranges_ex9()
{
    test_1();
}
