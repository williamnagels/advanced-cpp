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
    std::string status; // "Pending", "Completed", "Cancelled"
};

/*
GOAL:
The goal of this exercise is to transition from manual "Do-It-All" loops to 
composable standard algorithms.

1. Use std::copy_if and std::back_inserter to copy only completed orders into
    completedOrders.
2. Use std::accumulate to sum totalPrice in the intermediate container.

Note how the STL requires you to manage a temporary container to hold 
intermediate results.
*/
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
    for (const auto& o : orders) {
        if (o.status == "Completed") {
            rawSum += o.totalPrice;
        }
    } 
    assert(rawSum == 360.0);
    std::vector<Order> completedOrders;

    // TODO: Copy completed orders with std::copy_if.

    double stlSum = 0.0;
    // TODO: Calculate stlSum with std::accumulate and a binary lambda.
    
    // Uncomment once the exercise has been implemented.
    // assert(completedOrders.size() == 3);
    // assert(completedOrders.front().id == 1);
    // assert(completedOrders.back().id == 5);
    // assert(stlSum == rawSum);
}
}
void ranges_ex1()
{
    test_1();
}
