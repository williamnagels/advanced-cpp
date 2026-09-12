#include "pipeline.hpp"

#include <benchmark/benchmark.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <random>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace
{
constexpr int minimum = 1'000;

enum class InputPattern {
    all_pass,
    none_pass,
    alternating,
    random_50
};

using Pipeline = std::uint64_t (*)(std::span<const int>, int);

std::vector<int> make_input(std::size_t size, InputPattern pattern)
{
    std::vector<int> values(size);
    std::mt19937 generator(0xC0FFEE);
    std::bernoulli_distribution passes(0.5);

    for (std::size_t index = 0; index < size; ++index) {
        const int accepted = 1'000 + 2 * static_cast<int>(index % 512);
        const int rejected = 1 + 2 * static_cast<int>(index % 499);

        switch (pattern) {
        case InputPattern::all_pass:
            values[index] = accepted;
            break;
        case InputPattern::none_pass:
            values[index] = rejected;
            break;
        case InputPattern::alternating:
            values[index] = index % 2 == 0 ? accepted : rejected;
            break;
        case InputPattern::random_50:
            values[index] = passes(generator) ? accepted : rejected;
            break;
        }
    }
    return values;
}

void run_case(benchmark::State& state, Pipeline pipeline, InputPattern pattern, std::size_t size)
{
    //  Prepare data
    auto values = make_input(size, pattern);
    const auto expected = raw_pipeline(values, minimum);
    
    // Check if both pipelines wouldve returned the same data
    if (ranges_pipeline(values, minimum) != expected) {
        state.SkipWithError("raw and ranges pipelines disagree");
        return;
    }

    // Run the benchmark
    for (auto _ : state) {
        benchmark::DoNotOptimize(values.data());
        auto result = pipeline(values, minimum);
        benchmark::DoNotOptimize(result);
    }

    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(values.size()));
    state.SetBytesProcessed(state.iterations() * static_cast<std::int64_t>(values.size() * sizeof(int)));
}

constexpr std::array sizes{
    std::size_t{1'024},
    std::size_t{65'536},
    std::size_t{4'194'304}
};

struct PatternCase {
    InputPattern pattern;
    std::string_view name;
};

constexpr std::array patterns{
    PatternCase{InputPattern::all_pass, "all-pass"},
    PatternCase{InputPattern::none_pass, "none-pass"},
    PatternCase{InputPattern::alternating, "alternating"},
    PatternCase{InputPattern::random_50, "random-50"}
};

void register_benchmarks()
{
    constexpr std::array pipelines{
        std::pair{std::string_view{"raw"}, &raw_pipeline},
        std::pair{std::string_view{"ranges"}, &ranges_pipeline}
    };

    for (const auto& [pipelineName, pipeline] : pipelines) {
        for (const auto& pattern : patterns) {
            for (std::size_t size : sizes) {
                const auto name = std::string(pipelineName) + "/" +
                                  std::string(pattern.name) + "/" +
                                  std::to_string(size);
                benchmark::RegisterBenchmark(
                    name, run_case, pipeline, pattern.pattern, size)
                    ->MinTime(0.2)
                    ->Repetitions(9)
                    ->ReportAggregatesOnly(true);
            }
        }
    }
}
}

int main(int argc, char** argv)
{
    register_benchmarks();
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1;
    }
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}