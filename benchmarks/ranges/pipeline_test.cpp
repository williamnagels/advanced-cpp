#include "pipeline.hpp"

#include <array>
#include <span>

int main()
{
    constexpr std::array values{999, 1'000, 1'001, 1'002};
    constexpr std::uint64_t expected = (1'000U * 3U + 7U) +
                                       (1'002U * 3U + 7U);

    if (raw_pipeline(values, 1'000) != expected ||
        ranges_pipeline(values, 1'000) != expected) {
        return 1;
    }

    constexpr std::array<int, 0> empty{};
    if (raw_pipeline(empty, 1'000) != 0 ||
        ranges_pipeline(empty, 1'000) != 0) {
        return 1;
    }
}