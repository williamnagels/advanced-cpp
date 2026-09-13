#include <vector>
#include <ranges>
#include <cassert>
#include <concepts>
#include <type_traits>
#include <utility>

namespace
{
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
        //     - This is intentionally an input range: dereference returns a computed
        // value and the accumulated state changes as the iterator advances.
        // Also V is constrained as a std::ranges::input_range of arithmetic types.
        /*
        using iterator_concept = std::conditional_t<
            std::ranges::forward_range<V>,
            std::forward_iterator_tag,
            std::input_iterator_tag>;
        Notice how a randomaccess range is a forward range, and therefore would select
        std::forward_iterator_tag in the conditional above.
        Cant do randomaccess because we need to see each element in order to compute the running total.
        */
        using iterator_concept = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::ranges::range_value_t<V>;

        std::ranges::iterator_t<V> current_;
        std::ranges::sentinel_t<V> end_;
        /*
            - The running total belongs to each iterator, not the view. Two iterators
              can therefore have independent traversal state.
        */
        value_type total_{};

        iterator() = default;
        iterator(std::ranges::iterator_t<V> current,
                 std::ranges::sentinel_t<V> end)
            : current_(current), end_(end) {
            if (current_ != end_) {
                total_ = *current_;
            }
        }

        value_type operator*() const {
            return total_;
        }

        iterator& operator++() {
            ++current_;
            if (current_ != end_) {
                total_ += *current_;
            }
            return *this;
        }

        iterator operator++(int) {
            auto previous = *this;
            ++*this;
            return previous;
        }

        bool operator==(std::default_sentinel_t) const {
            return current_ == end_;
        }
    };

    iterator begin() {
        return {std::ranges::begin(base_), std::ranges::end(base_)};
    }

    std::default_sentinel_t end() const noexcept { return {}; }
};

// CTAD selects ref_view for lvalues and owning_view for movable rvalues.
// - views::all is the lifetime boundary: lvalues become ref_view, while
//   movable rvalues become owning_view.
template<std::ranges::viewable_range R>
running_total_view(R&&) -> running_total_view<std::views::all_t<R>>;

struct running_total_closure
    : std::ranges::range_adaptor_closure<running_total_closure> {
    // Deriving from range_adaptor_closure supplies the range | adaptor syntax.
    auto operator()(std::ranges::viewable_range auto&& range) const {
        return running_total_view(
            std::views::all(std::forward<decltype(range)>(range)));
    }
};

inline constexpr running_total_closure running_total;

void test_1()
{
    std::vector<double> samples = {10, 13, 12, 20};

    auto totals = samples | running_total;

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
}
}

void ranges_ex5()
{
    test_1();
}