#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <iterator>
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
    double rawSum = 0;

        /*
        - Start with the loop: it combines selection and aggregation in one place.
            Is that clearer here, or does separating the operations improve intent?
        */
    for (const auto& order : orders) {
        if (order.status == "Completed") {
            rawSum += order.totalPrice;
        }
    }
    assert(rawSum == 360.0);
    std::vector<Order> completedOrders;

    // back_inserter lets copy_if grow the destination without pre-sizing it.
    /*
        - copy_if materializes completedOrders. This is useful when the selected
            orders are reused, but wasteful when only the sum is needed.
    */
    std::copy_if(orders.begin(), orders.end(),
                 std::back_inserter(completedOrders),
                 [](const Order& order) {
                     return order.status == "Completed";
                 });

    // The accumulator carries the running price while each Order is visited.
    /*
        - accumulate combines selection and aggregation in one pass, avoiding
            the need for an intermediate container.

        double sum = std::accumulate(
            orders.begin(), orders.end(), 0.0,
            [](double total, const Order& order) {
                return order.status == "Completed"
                    ? total + order.totalPrice
                    : total;
            });
    */
    double stlSum = std::accumulate(
        completedOrders.begin(), completedOrders.end(), 0.0,
        [](double sum, const Order& order) {
            return sum + order.totalPrice;
        });

    assert(completedOrders.size() == 3);
    assert(completedOrders.front().id == 1);
    assert(completedOrders.back().id == 5);
    assert(stlSum == rawSum);
}
}

void ranges_ex1()
{
    test_1();
}