#include <ranges>
#include <vector>
#include <numeric>
#include <cassert>
#include <iostream>
#include <type_traits>
#include <deque>
template <std::ranges::input_range V>
requires std::ranges::view<V> && std::is_arithmetic_v<std::ranges::range_value_t<V>>
class moving_average_view
    : public std::ranges::view_interface<moving_average_view<V>>
{
    V base_;
    std::size_t window_;
    using T = std::ranges::range_value_t<V>;
public:
    moving_average_view() = default;
    template <std::ranges::viewable_range R>
    moving_average_view(R&& r, std::size_t w)
        : base_(std::forward<R>(r)), window_(w) {}
    struct iterator {
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::input_iterator_tag;
        std::ranges::iterator_t<V> it_;
        std::ranges::sentinel_t<V> end_;
        std::size_t window_;
        std::deque<T> buf_;
        T sum_{0};
        bool done_{false};
        iterator() = default;
        iterator(std::ranges::iterator_t<V> it,
                 std::ranges::sentinel_t<V> end,
                 std::size_t window)
            : it_(it), end_(end), window_(window)
        {
            // Pre-fill
            while (buf_.size() < window_ && it_ != end_) {
                sum_ += *it_;
                buf_.push_back(*it_);
                ++it_;
            }
            if (buf_.size() < window_) {
                done_ = true;
            }
        }
        value_type operator*() const {
            return sum_ / static_cast<T>(buf_.size());
        }
        iterator& operator++() {
            if (done_) return *this;
            if (it_ == end_) {
                done_ = true;
                return *this;
            }
            sum_ -= buf_.front();
            buf_.pop_front();
            sum_ += *it_;
            buf_.push_back(*it_);
            ++it_;
            return *this;
        }
        void operator++(int) { ++(*this); }
        bool operator==(std::default_sentinel_t) const {
            return done_;
        }
    };
    iterator begin() {
        return iterator(std::ranges::begin(base_), std::ranges::end(base_), window_);
    }
    std::default_sentinel_t end() const noexcept {
        return {};
    }
};
template <class R>
moving_average_view(R&&, std::size_t)
-> moving_average_view<std::views::all_t<R>>;
struct moving_average {
    std::size_t window_;
    friend auto operator|(std::ranges::viewable_range auto&& r, moving_average const& self) {
        return moving_average_view(std::forward<decltype(r)>(r), self.window_);
    }
};
int main() {
    std::vector<double> data = {1, 2, 3, 4, 5, 6};
    auto avg3 = data | moving_average(3);
    static_assert(std::same_as<decltype(avg3), moving_average_view<std::ranges::ref_view<std::vector<double>>>>);
    auto avg4 = std::move(data) | moving_average(3);
    static_assert(std::same_as<decltype(avg4), moving_average_view<std::ranges::owning_view<std::vector<double>>>>);
    return 0;
}
