#include <vector>
#include <ranges>
#include <cassert>
#include <deque>
#include <type_traits>
#include <utility>

namespace
{
/*
GOAL:
Implement the moving_average view and adaptor introduced in the slides.

For input {1, 2, 3, 4, 5} and a window size of 3, iteration produces
{2, 3, 4}. The result remains lazy and no result vector is allocated.

1. Complete moving_average_view's input iterator. Pre-fill its deque with one
   complete window and keep a running sum.
2. End iteration when no complete window can be formed.
3. Add the deduction guide using std::views::all_t so lvalues become ref_view
   and rvalues become owning_view.
4. Complete the adaptor closure so range | moving_average(3) works.

Assume a positive window size. This exercise focuses on view storage, iterator
state, CTAD, and range_adaptor_closure rather than argument validation.
*/
/* TODO: Uncomment and complete the view.
template<std::ranges::input_range V>
requires std::ranges::view<V> &&
         std::is_arithmetic_v<std::ranges::range_value_t<V>>
class moving_average_view
    : public std::ranges::view_interface<moving_average_view<V>> {
    V base_;
    std::size_t windowSize_;

public:
    moving_average_view() = default;
    moving_average_view(V base, std::size_t windowSize)
        : base_(std::move(base)), windowSize_(windowSize) {}

    struct iterator {
        using iterator_concept = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::ranges::range_value_t<V>;

        std::ranges::iterator_t<V> current_;
        std::ranges::sentinel_t<V> end_;
        std::deque<value_type> window_;
        value_type sum_{};
        std::size_t windowSize_{};
        bool done_{};

        iterator() = default;
        iterator(std::ranges::iterator_t<V> current,
                 std::ranges::sentinel_t<V> end,
                 std::size_t windowSize) {
            // TODO: Store the arguments and pre-fill one complete window.
        }

        value_type operator*() const {
            // TODO: Return sum_ divided by window_.size().
        }
        iterator& operator++() {
            // TODO: Slide by one element, maintaining window_ and sum_.
        }
        void operator++(int) { ++*this; }
        bool operator==(std::default_sentinel_t) const { return done_; }
    };

    iterator begin() {
        return {std::ranges::begin(base_), std::ranges::end(base_), windowSize_};
    }
    std::default_sentinel_t end() const noexcept { return {}; }
};

// TODO: Add a deduction guide using std::views::all_t<R>.

struct moving_average_closure
    : std::ranges::range_adaptor_closure<moving_average_closure> {
    std::size_t windowSize;

    explicit moving_average_closure(std::size_t size) : windowSize(size) {}

    auto operator()(std::ranges::viewable_range auto&& range) const {
        // TODO: Construct moving_average_view from views::all(range).
    }
};

inline auto moving_average(std::size_t windowSize) {
    return moving_average_closure{windowSize};
}
*/

void test_1() {
    std::vector<double> samples = {1, 2, 3, 4, 5};

    // TODO: Uncomment once moving_average has been implemented.
    // auto averages = samples | moving_average(3);

    /* Uncomment once the exercise has been implemented.
    static_assert(std::ranges::input_range<decltype(averages)>);
    static_assert(std::same_as<
        decltype(averages),
        moving_average_view<std::ranges::ref_view<std::vector<double>>>>);

    auto it = averages.begin();
    assert(*it++ == 2.0);
    assert(*it++ == 3.0);
    assert(*it++ == 4.0);
    assert(it == averages.end());

    auto owned = std::vector<double>{2, 4, 6} | moving_average(2);
    static_assert(std::same_as<
        decltype(owned),
        moving_average_view<std::ranges::owning_view<std::vector<double>>>>);
    assert(*owned.begin() == 3.0);
    */
}
}
void ranges_ex5()
{
    test_1();
}