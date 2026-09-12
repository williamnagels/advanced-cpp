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
pairwise_difference view.

For input {10, 13, 12, 20}, iteration produces {3, -1, 8}. Each value is the
difference between two adjacent source elements. The result remains lazy and
no result vector is allocated.

1. Complete pairwise_difference_view's input iterator. Store the previous
   value and position the source iterator at the next value.
2. Dereference to calculate current - previous, then advance both values.
3. End iteration when there is no next source element.
4. Add the deduction guide using std::views::all_t so lvalues become ref_view
   and rvalues become owning_view.
5. Complete the adaptor closure so range | pairwise_difference works.

This exercise focuses on transferring the view storage, iterator state, CTAD,
and range_adaptor_closure patterns to a new lazy transformation.
*/
/* TODO: Uncomment and complete the view.
template<std::ranges::input_range V>
requires std::ranges::view<V> &&
         std::is_arithmetic_v<std::ranges::range_value_t<V>>
class pairwise_difference_view
    : public std::ranges::view_interface<pairwise_difference_view<V>> {
    V base_;

public:
    pairwise_difference_view() = default;
    explicit pairwise_difference_view(V base) : base_(std::move(base)) {}

    struct iterator {
        using iterator_concept = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::ranges::range_value_t<V>;

        std::ranges::iterator_t<V> current_;
        std::ranges::sentinel_t<V> end_;
        value_type previous_{};
        bool done_{};

        iterator() = default;
        iterator(std::ranges::iterator_t<V> current,
                 std::ranges::sentinel_t<V> end) {
            // TODO: Store the arguments, save the first value in previous_,
            // and advance current_ to the second value when one exists.
        }

        value_type operator*() const {
            // TODO: Return *current_ - previous_.
        }
        iterator& operator++() {
            // TODO: Move the current value into previous_, then advance.
        }
        void operator++(int) { ++*this; }
        bool operator==(std::default_sentinel_t) const { return done_; }
    };

    iterator begin() {
        return {std::ranges::begin(base_), std::ranges::end(base_)};
    }
    std::default_sentinel_t end() const noexcept { return {}; }
};

// TODO: Add a deduction guide using std::views::all_t<R>.

struct pairwise_difference_closure
    : std::ranges::range_adaptor_closure<pairwise_difference_closure> {
    auto operator()(std::ranges::viewable_range auto&& range) const {
        // TODO: Construct pairwise_difference_view from views::all(range).
    }
};

inline constexpr pairwise_difference_closure pairwise_difference;
*/

void test_1() {
    std::vector<double> samples = {10, 13, 12, 20};

    // TODO: Uncomment once pairwise_difference has been implemented.
    // auto differences = samples | pairwise_difference;

    /* Uncomment once the exercise has been implemented.
    static_assert(std::ranges::input_range<decltype(differences)>);
    static_assert(std::same_as<
        decltype(differences),
        pairwise_difference_view<std::ranges::ref_view<std::vector<double>>>>);

    auto it = differences.begin();
    assert(*it++ == 3.0);
    assert(*it++ == -1.0);
    assert(*it++ == 8.0);
    assert(it == differences.end());

    auto owned = std::vector<double>{2, 4, 7} | pairwise_difference;
    static_assert(std::same_as<
        decltype(owned),
        pairwise_difference_view<std::ranges::owning_view<std::vector<double>>>>);
    assert(*owned.begin() == 2.0);
    */
}
}
void ranges_ex5()
{
    test_1();
}