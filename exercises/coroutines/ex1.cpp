#include <algorithm>
#include <cassert>
#include <generator>
#include <iterator>
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
3. Use std::ranges::copy and std::back_inserter to collect the generated stages.
4. Enable the assertions for deployments with and without tests.

Observe that calling deployment_stages only creates a lazy generator. Its body
runs as the ranges algorithm requests each value.
*/
std::generator<std::string_view> deployment_stages(bool run_tests)
{
    (void)run_tests;
    co_return;

    // TODO: Replace the placeholder above with the deployment stages.
}

void test_1()
{
    auto with_tests_stream = deployment_stages(true);
    auto without_tests_stream = deployment_stages(false);
    std::vector<std::string_view> with_tests;
    std::vector<std::string_view> without_tests;

    // TODO: Copy with_tests_stream into with_tests.
    // TODO: Copy without_tests_stream into without_tests.
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
