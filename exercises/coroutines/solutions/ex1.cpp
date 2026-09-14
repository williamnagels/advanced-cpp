#include <cassert>
#include <generator>
#include <iterator>
#include <ranges>
#include <string_view>
#include <vector>

namespace
{
std::generator<std::string_view> deployment_stages(bool run_tests)
{
    co_yield "configure";
    co_yield "build";
    if (run_tests)
        co_yield "test";
    co_yield "deploy";
}

using DeploymentStages = decltype(deployment_stages(false));
using DeploymentIterator = std::ranges::iterator_t<DeploymentStages>;

static_assert(std::ranges::range<DeploymentStages>);
static_assert(std::ranges::view<DeploymentStages>);
static_assert(std::ranges::input_range<DeploymentStages>);
static_assert(!std::ranges::forward_range<DeploymentStages>);
static_assert(!std::ranges::common_range<DeploymentStages>);
static_assert(!std::ranges::sized_range<DeploymentStages>);

static_assert(std::input_iterator<DeploymentIterator>);
static_assert(!std::forward_iterator<DeploymentIterator>);

void test_1()
{
    auto with_tests_stream = deployment_stages(true);
    auto without_tests_stream = deployment_stages(false);
    auto with_tests = std::ranges::to<std::vector>(with_tests_stream);
    auto without_tests = std::ranges::to<std::vector>(without_tests_stream);

    assert((with_tests == std::vector<std::string_view>{
        "configure", "build", "test", "deploy"}));
    assert((without_tests == std::vector<std::string_view>{
        "configure", "build", "deploy"}));
}
}

void coroutines_ex1()
{
    test_1();
}
