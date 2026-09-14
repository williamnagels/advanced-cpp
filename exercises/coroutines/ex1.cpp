#include <cassert>
#include <generator>
#include <iterator>
#include <ranges>
#include <string_view>
#include <vector>

namespace
{
/*
GOAL:
Use std::generator before looking at coroutine implementation details. Produce a
lazy sequence of deployment stages and consume it as an ordinary input range.

1. Replace the placeholder body in deployment_stages with co_yield expressions
   for "configure", "build", "test", and "deploy".
2. Yield "test" only when run_tests is true.
3. Before consuming the generator, predict which range concepts it models:
    view, input_range, forward_range, common_range, and sized_range. What
    iterator concept does its iterator model? Add static_asserts to test your
    predictions.
4. Use std::ranges::to to collect each generated range into a vector.
5. Enable the assertions for deployments with and without tests.

Observe that calling deployment_stages only creates a lazy generator. Its body
runs as the ranges algorithm requests each value.
*/
std::generator<std::string_view> deployment_stages(bool run_tests)
{
    (void)run_tests;
    co_return;

    // TODO: Replace the placeholder above with the deployment stages.
}

using DeploymentStages = decltype(deployment_stages(false));
static_assert(std::ranges::range<DeploymentStages>);

// TODO: Use std::ranges::iterator_t and more static_asserts to investigate the
// generator's remaining range and iterator concepts.

void test_1()
{
    auto with_tests_stream = deployment_stages(true);
    auto without_tests_stream = deployment_stages(false);
    std::vector<std::string_view> with_tests;
    std::vector<std::string_view> without_tests;

    // TODO: Convert each stream with std::ranges::to.
    (void)with_tests_stream;
    (void)without_tests_stream;

    // Uncomment once the exercise has been implemented.
    // assert((with_tests == std::vector<std::string_view>{
    //     "configure", "build", "test", "deploy"}));
    // assert((without_tests == std::vector<std::string_view>{
    //     "configure", "build", "deploy"}));
}
}

void coroutines_ex1()
{
    test_1();
}
