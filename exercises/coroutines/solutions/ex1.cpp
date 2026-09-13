#include <algorithm>
#include <cassert>
#include <generator>
#include <iterator>
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

void test_1()
{
    auto with_tests_stream = deployment_stages(true);
    auto without_tests_stream = deployment_stages(false);
    std::vector<std::string_view> with_tests;
    std::vector<std::string_view> without_tests;

    std::ranges::copy(with_tests_stream, std::back_inserter(with_tests));
    std::ranges::copy(without_tests_stream, std::back_inserter(without_tests));

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
