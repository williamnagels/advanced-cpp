#include <vector>
#include <ranges>
#include <cassert>
#include <type_traits>
#include <utility>

namespace
{
/*
GOAL:
Apply the custom-view pattern from the slides to a different problem: a
running_total view.

For input {10, 13, 12, 20}, iteration produces {10, 23, 35, 55}. Each value is
the sum of all source values seen so far. The result remains lazy and no result
vector is allocated.

1. Complete running_total_view's input iterator. Store the current source
    position and accumulated total.
2. Dereference to return the total, then advance and add the next value.
3. End iteration when the source iterator reaches its sentinel.
4. Add the deduction guide using std::views::all_t so lvalues become ref_view
   and rvalues become owning_view.
5. Complete the adaptor closure so range | running_total works.

This exercise focuses on transferring the view storage, iterator state, CTAD,
and range_adaptor_closure patterns to a new lazy transformation.
*/
/* TODO: Uncomment and complete the view.
template<std::ranges::input_range V>
requires std::ranges::view<V> &&
         std::is_arithmetic_v<std::ranges::range_value_t<V>>
class running_total_view
    : public std::ranges::view_interface<running_total_view<V>> {
    V base_;

public:
    running_total_view() = default;
    explicit running_total_view(V base) : base_(std::move(base)) {}

    struct iterator {
        using iterator_concept = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::ranges::range_value_t<V>;

        std::ranges::iterator_t<V> current_;
        std::ranges::sentinel_t<V> end_;
        value_type total_{};

        iterator() = default;
        iterator(std::ranges::iterator_t<V> current,
                 std::ranges::sentinel_t<V> end) {
            // TODO: Store the arguments and initialize total_ from the first
            // value when the source is not empty.
        }

        value_type operator*() const {
            // TODO: Return the accumulated total.
        }
        iterator& operator++() {
            // TODO: Advance current_, then add its value when not at end_.
        }
        void operator++(int) { ++*this; }
        bool operator==(std::default_sentinel_t) const {
            return current_ == end_;
        }
    };

    iterator begin() {
        return {std::ranges::begin(base_), std::ranges::end(base_)};
    }
    std::default_sentinel_t end() const noexcept { return {}; }
};

// TODO: Add a deduction guide using std::views::all_t<R>.

struct running_total_closure
    : std::ranges::range_adaptor_closure<running_total_closure> {
    auto operator()(std::ranges::viewable_range auto&& range) const {
        // TODO: Construct running_total_view from views::all(range).
    }
};

inline constexpr running_total_closure running_total;
*/

void test_1() {
    std::vector<double> samples = {10, 13, 12, 20};

    // TODO: Uncomment once running_total has been implemented.
    // auto totals = samples | running_total;

    /* Uncomment once the exercise has been implemented.
    static_assert(std::ranges::input_range<decltype(totals)>);
    static_assert(std::same_as<
        decltype(totals),
        running_total_view<std::ranges::ref_view<std::vector<double>>>>);

    auto it = totals.begin();
    assert(*it++ == 10.0);
    assert(*it++ == 23.0);
    assert(*it++ == 35.0);
    assert(*it++ == 55.0);
    assert(it == totals.end());

    auto owned = std::vector<double>{2, 4, 7} | running_total;
    static_assert(std::same_as<
        decltype(owned),
        running_total_view<std::ranges::owning_view<std::vector<double>>>>);
    assert(*owned.begin() == 2.0);
    */
}
}
void ranges_ex5()
{
    test_1();
}