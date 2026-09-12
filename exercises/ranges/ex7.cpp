#include <algorithm>
#include <cassert>
#include <ranges>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace
{
/*
TODO: Implement a view that greedily groups adjacent costs into batches whose
sum does not exceed a budget. A single item that exceeds the budget forms its
own batch so iteration always makes progress.

For {4, 2, 5, 3, 1} with budget 7, produce these subranges:
    {4, 2}, {5}, {3, 1}

Constrain V to be a view whose const form is both a forward_range and a
common_range. Looking ahead must not consume the source, and each batch is
represented by two iterators of the same type.

template<std::ranges::view V>
// TODO: Constrain V to be a view whose const form is both a forward_range and a common_range, and whose value type is arithmetic.
class budget_batches_view
    : public std::ranges::view_interface<budget_batches_view<V>> {
    V base_;
    std::ranges::range_value_t<const V> budget_{};

public:
    budget_batches_view() = default;

    template<std::ranges::viewable_range R>
    budget_batches_view(R&& range,
                        std::ranges::range_value_t<const V> budget)
        : base_(std::views::all(std::forward<R>(range))), budget_(budget) {
        // TODO: Reject a negative budget with std::invalid_argument.
    }

    struct iterator {
        using source_iterator = std::ranges::iterator_t<const V>;
        using iterator_category = std::forward_iterator_tag;
        using iterator_concept = std::forward_iterator_tag;
        using value_type = std::ranges::subrange<source_iterator>;
        using difference_type = std::ranges::range_difference_t<const V>;
        using pointer = void;
        using reference = value_type;

        source_iterator current_{};
        source_iterator batchEnd_{};
        source_iterator end_{};
        std::ranges::range_value_t<const V> budget_{};

        iterator() = default;
        iterator(source_iterator current, source_iterator end,
                 std::ranges::range_value_t<const V> budget)
            : current_(current), batchEnd_(current), end_(end), budget_(budget) {
            // TODO: Find the end of the first greedy batch.
        }

        value_type operator*() const {
            // TODO: Return [current_, batchEnd_).
        }

        iterator& operator++() {
            // TODO: Start at batchEnd_ and find the end of the next batch.
        }

        iterator operator++(int) {
            auto previous = *this;
            ++*this;
            return previous;
        }

        bool operator==(const iterator&) const = default;
        bool operator==(std::default_sentinel_t) const {
            return current_ == end_;
        }
    };

    iterator begin() const {
        return {std::ranges::begin(base_), std::ranges::end(base_), budget_};
    }

    std::default_sentinel_t end() const noexcept { return {}; }
};

template<std::ranges::viewable_range R>
budget_batches_view(R&&, std::ranges::range_value_t<R>)
    -> budget_batches_view<std::views::all_t<R>>;
*/
}

// TODO: The iterator contains source iterators and its budget, not a pointer to
// the view object, so borrowing can follow the underlying view.
// define enable_borrowed_range for budget_batches_view based on the underlying view.
/*
namespace std::ranges {

}
*/

namespace
{
void test_budget_batches() {
    std::vector<int> values = {4, 2, 5, 3, 1};
    std::span source{values};

    /* TODO: Uncomment once budget_batches_view has been implemented.
    auto batches = budget_batches_view(source, 7);
    auto it = batches.begin();

    assert(std::ranges::equal(*it++, std::vector{4, 2}));
    assert(std::ranges::equal(*it++, std::vector{5}));
    assert(std::ranges::equal(*it++, std::vector{3, 1}));
    assert(it == batches.end());

    auto found = std::ranges::find_if(std::move(batches), [](auto batch) {
        return std::ranges::equal(batch, std::vector{5});
    });
    static_assert(!std::same_as<decltype(found), std::ranges::dangling>);
    assert(std::ranges::equal(*found, std::vector{5}));
    */
}

void test_dangling_batches() {
    /* TODO: Uncomment once budget_batches_view has been implemented.
    auto batches = budget_batches_view(std::vector{4, 2, 5}, 7);
    auto found = std::ranges::find_if(std::move(batches), [](auto) {
        return true;
    });
    static_assert(std::same_as<decltype(found), std::ranges::dangling>);
    */
}
}

/*
GOAL:
Implement budget_batches_view, a domain-specific lazy grouping operation with
no direct standard view equivalent.

1. Require a const forward, common source range so look-ahead is non-consuming
   and each result can be represented as a subrange.
2. Greedily find each batch boundary while guaranteeing forward progress.
3. Return source subranges without allocating result containers.
4. Propagate borrowed_range only when the underlying view is borrowed.
*/
void ranges_ex7()
{
    test_budget_batches();
    test_dangling_batches();
}
