#include "pipeline.hpp"

#include <ranges>

namespace
{
struct transform_value_fn {
    constexpr std::uint64_t operator()(int value) const
    {
        return static_cast<std::uint64_t>(value) * 3U + 7U;
    }
};

inline constexpr transform_value_fn transform_value;
}

[[gnu::noinline]]
std::uint64_t raw_pipeline(std::span<const int> values, int minimum)
{
    std::uint64_t sum = 0;
    for (int value : values) {
        if (value >= minimum && value % 2 == 0) {
            sum += transform_value(value);
        }
    }
    return sum;
}

[[gnu::noinline]]
std::uint64_t ranges_pipeline(std::span<const int> values, int minimum)
{
    auto transformed = values
        | std::views::filter([minimum](int value) {
              return value >= minimum && value % 2 == 0;
          })
        | std::views::transform(transform_value);

    std::uint64_t sum = 0;
    for (std::uint64_t value : transformed) {
        sum += value;
    }
    return sum;
}