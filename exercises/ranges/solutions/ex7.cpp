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
template<std::ranges::view V>
requires std::ranges::forward_range<const V> &&
         std::ranges::common_range<const V> &&
         std::is_arithmetic_v<std::ranges::range_value_t<const V>>
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
        if constexpr (std::is_signed_v<std::ranges::range_value_t<const V>>) {
            if (budget_ < 0) {
                throw std::invalid_argument("budget must not be negative");
            }
        }
    }

    struct iterator {
        using source_iterator = std::ranges::iterator_t<const V>;
        using iterator_concept = std::forward_iterator_tag;
        using value_type = std::ranges::subrange<source_iterator>;
        using difference_type = std::ranges::range_difference_t<const V>;

        source_iterator current_{};
        source_iterator batchEnd_{};
        source_iterator end_{};
        std::ranges::range_value_t<const V> budget_{};

        iterator() = default;
        iterator(source_iterator current, source_iterator end,
                 std::ranges::range_value_t<const V> budget)
            : current_(current), batchEnd_(current), end_(end), budget_(budget) {
            find_batch_end();
        }

        value_type operator*() const {
            return {current_, batchEnd_};
        }

        iterator& operator++() {
            current_ = batchEnd_; // start is now at the beginning of the next batch
            find_batch_end(); // look for new batch end
            return *this;
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

    private:
        void find_batch_end() {
            batchEnd_ = current_;
            if (batchEnd_ == end_) {
                return;
            }

            // Always consume one item, even when that item exceeds the budget.
            auto sum = *batchEnd_;
            ++batchEnd_;
            while (batchEnd_ != end_ && sum + *batchEnd_ <= budget_) {
                sum += *batchEnd_;
                ++batchEnd_;
            }
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
}

// Iterator lifetime follows the source because it stores source iterators only.
namespace std::ranges {
template<class V>
inline constexpr bool enable_borrowed_range<::budget_batches_view<V>> =
    enable_borrowed_range<V>;
}

/*
INSTRUCTOR NOTE:
Borrowed-ness follows the wrapped view. A span-backed temporary view can return
usable iterators, but an owning_view temporary cannot. Compare the two
static_asserts below and ask what object actually owns the elements.
*/

namespace
{
void test_budget_batches()
{
    std::vector<int> values = {4, 2, 5, 3, 1};
    std::span source{values};

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
}

void test_dangling_batches()
{
    auto batches = budget_batches_view(std::vector{4, 2, 5}, 7);
    auto found = std::ranges::find_if(std::move(batches), [](auto) {
        return true;
    });
    static_assert(std::same_as<decltype(found), std::ranges::dangling>);
}
}

void ranges_ex7()
{
    test_budget_batches();
    test_dangling_batches();
}