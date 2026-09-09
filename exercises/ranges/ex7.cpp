#include <ranges>
#include <algorithm>
#include <vector>
#include <span>
#include <cassert>
#include <stdexcept>

namespace
{
/*
TODO: Constrain V to be a view whose const form is both a forward_range and a
common_range. Looking ahead must not consume the source, and each window is
represented by two iterators of the same type.

template<... V>
requires ...
class sliding_window_view : public std::ranges::view_interface<sliding_window_view<V>> {
    // TODO: Store V base_ and a range_difference_t<const V> windowSize_.
public:
    // TODO: Add a default constructor and a constructor taking V and window size.
    // Reject a window size smaller than one with std::invalid_argument.

    struct iterator {
        // Store current_, windowEnd_, and end_. Keeping windowEnd_ makes
        // increment constant-time instead of rescanning each window.
        using iterator_category = std::forward_iterator_tag;
        using iterator_concept = std::forward_iterator_tag;
        using value_type = std::ranges::subrange<std::ranges::iterator_t<const V>>;
        using difference_type = std::ranges::range_difference_t<const V>;
        using pointer = void;
        using reference = value_type;

        // TODO: Add constructors, including the default constructor required by
        // forward_iterator.

        value_type operator*() const {
            // TODO: Return [current_, windowEnd_).
        }

        iterator& operator++() {
            // TODO: Advance both iterators. After yielding the final complete
            // window, move current_ to end_ so it equals the sentinel.
        }
        iterator operator++(int) {
            // TODO: Implement postfix increment in terms of prefix increment.
        }
        bool operator==(const iterator& other) const = default;
        bool operator==(std::default_sentinel_t) const {
            // TODO: No complete window remains when current_ equals end_.
        }
    };

    iterator begin() const {
        // TODO: Find the end of the first window with std::ranges::next.
        // Return the end iterator immediately if a full window does not fit.
    }
    std::default_sentinel_t end() const noexcept {
        return {};
    }
};
*/
}

// TODO: This specialization must be in std::ranges, but the exercise's view
// type itself remains in the anonymous namespace.
/*
namespace std::ranges {
template<typename V>
inline constexpr bool enable_borrowed_range<::sliding_window_view<V>> =
    enable_borrowed_range<V>;
}
*/

namespace
{
void test_sliding_window() {
    std::vector<int> values = {1, 2, 3, 4, 5};
    std::span source{values};

    /*
    TODO: Uncomment once sliding_window_view has been implemented.

    auto windows = sliding_window_view(std::views::all(source), 3);
    auto it = std::ranges::find_if(std::move(windows), [](auto window) {
        return *window.begin() == 2;
    });

    static_assert(!std::same_as<decltype(it), std::ranges::dangling>);
    assert(std::ranges::equal(*it, std::vector{2, 3, 4}));
    */
}

void test_dangling_window() {
    /*
    TODO: Uncomment once sliding_window_view has been implemented.

    auto makeValues = [] { return std::vector{1, 2, 3}; };
    auto windows = sliding_window_view(std::views::all(makeValues()), 2);
    auto it = std::ranges::find_if(std::move(windows), [](auto) { return true; });
    static_assert(std::same_as<decltype(it), std::ranges::dangling>);
    */
}
}

/*
GOAL:
Implement a sliding_window_view that produces subranges of size N, advancing
one element at a time.

1. Require at least a forward range because looking ahead must not consume it.
2. Keep the current window's begin and end so increment remains constant-time.
3. Stop when fewer than N elements remain.
4. Propagate borrowed_range only when the underlying view is borrowed.
*/
void ranges_ex7()
{
    test_sliding_window();
    test_dangling_window();
}