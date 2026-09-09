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
    std::string status; // "Pending", "Completed", "Cancelled"
};

/*
GOAL:
The goal of this exercise is to evolve from the algo STL approach to a 
functional pipeline using C++20 Ranges.

In the previous STL exercise, you may have used an extra container 
to manage memory by creating  a temporary 'completedOrders' vector. 
Ranges eliminate this overhead through "Lazy Evaluation."

1. Create a View Pipeline: Use the pipe operator (|) to chain 'std::views::filter' 
   and 'std::views::transform' together.
2. Filter & Project: Instead of copying objects, filter the stream for 
   "Completed" orders and then "transform" (project) the stream so it only 
   contains the 'totalPrice' doubles.
3. Consume the view with std::accumulate without creating an intermediate
    container. Notice that this works because this pipeline is a common_range.
*/
void test_1() {
    std::vector<Order> orders = {
        {1, 150.0, "Completed"},
        {2, 30.0,  "Pending"},
        {3, 200.0, "Completed"},
        {4, 50.0,  "Cancelled"},
        {5, 10.0,  "Completed"}
    };
    
    // TODO: Build completedPrices with std::views::filter and
    // std::views::transform. Keep the Order objects in orders.
    // auto completedPrices = ...;

    double rangesSum = 0.0;
    // TODO: Use std::accumulate(completedPrices.begin(),
    //                           completedPrices.end(), 0.0).

    // Uncomment once the exercise has been implemented.
    // static_assert(std::ranges::view<decltype(completedPrices)>);
    // assert(std::ranges::distance(completedPrices) == 3);
    // assert(rangesSum == 360.0);
}
}
void ranges_ex4()
{
    test_1();
}