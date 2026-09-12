#include <vector>
#include <string>
#include <cassert>
#include <numeric>
#include <ranges>

namespace
{
struct Order {
    int id;
    double totalPrice;
    std::string status;
};

void test_1()
{
    std::vector<Order> orders = {
        {1, 150.0, "Completed"},
        {2, 30.0,  "Pending"},
        {3, 200.0, "Completed"},
        {4, 50.0,  "Cancelled"},
        {5, 10.0,  "Completed"}
    };

    // Views filter and project lazily; no intermediate Order vector is built.
    auto completedPrices = orders
        | std::views::filter([](const Order& order) {
              return order.status == "Completed";
          })
        | std::views::transform([](const Order& order) {
              return order.totalPrice;
          });

    double rangesSum = std::accumulate(completedPrices.begin(),
                                       completedPrices.end(), 0.0);

    static_assert(std::ranges::view<decltype(completedPrices)>);
    assert(std::ranges::distance(completedPrices) == 3);
    assert(rangesSum == 360.0);
}
}

void ranges_ex4()
{
    test_1();
}