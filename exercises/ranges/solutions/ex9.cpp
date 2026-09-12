#include <vector>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>

namespace
{
template<std::ranges::view V, typename Pred>
requires std::ranges::bidirectional_range<V> &&
         std::ranges::common_range<V> &&
         std::indirect_unary_predicate<Pred, std::ranges::iterator_t<V>>
class trim_view : public std::ranges::view_interface<trim_view<V, Pred>> {
    V base_;
    // Empty predicates, such as stateless lambdas, need not increase view size.
    [[no_unique_address]] Pred predicate_;

public:
    trim_view() = default;
    trim_view(V base, Pred predicate)
        : base_(std::move(base)), predicate_(std::move(predicate)) {}

    auto begin() {
        return std::ranges::find_if_not(base_, std::ref(predicate_));
    }

    auto end() {
        auto first = begin();
        auto last = std::ranges::end(base_);
        while (last != first) {
            auto previous = last;
            --previous;
            
            // std::invoke makes trim_view compatible with the full range of predicates 
            // accepted by std::indirect_unary_predicate. 
            if (!std::invoke(predicate_, *previous)) {
                break;
            }
            last = previous;
        }
        return last;
    }
};

template<std::ranges::viewable_range R, typename Pred>
trim_view(R&&, Pred) -> trim_view<std::views::all_t<R>, Pred>;

template<typename Pred>
struct trim_closure : std::ranges::range_adaptor_closure<trim_closure<Pred>> {
    [[no_unique_address]] Pred predicate_;

    explicit trim_closure(Pred predicate) : predicate_(std::move(predicate)) {}

    template<std::ranges::viewable_range R>
    auto operator()(R&& range) const {
        return trim_view(std::views::all(std::forward<R>(range)), predicate_);
    }
};

struct trim_fn {
    template<std::ranges::viewable_range R, typename Pred>
    auto operator()(R&& range, Pred predicate) const {
        return trim_view(std::views::all(std::forward<R>(range)),
                         std::move(predicate));
    }

    template<typename Pred>
    auto operator()(Pred&& predicate) const {
        return trim_closure<std::decay_t<Pred>>(
            std::forward<Pred>(predicate));
    }
};

inline constexpr trim_fn trim;

void test_1()
{
    std::vector<int> data = {0, 0, 7, 8, 9, 0, 0};
    auto is_zero = [](int value) { return value == 0; };

    auto trimmed = data | trim(is_zero);
    auto empty = std::vector<int>{} | trim(is_zero);
    auto allZeros = std::vector{0, 0, 0} | trim(is_zero);

    assert(std::ranges::distance(trimmed) == 3);
    assert(*trimmed.begin() == 7);
    assert(*std::ranges::prev(trimmed.end()) == 9);
    assert(std::ranges::empty(empty));
    assert(std::ranges::empty(allZeros));
}
}

void ranges_ex9()
{
    test_1();
}