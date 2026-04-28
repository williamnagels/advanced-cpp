#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
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
3. Consuming the View: Use a reduction algorithm to process the resulting 
   view without ever creating an intermediate container.
*/
void test_1() {
    std::vector<Order> orders = {
        {1, 150.0, "Completed"},
        {2, 30.0,  "Pending"},
        {3, 200.0, "Completed"},
        {4, 50.0,  "Cancelled"},
        {5, 10.0,  "Completed"}
    };
    
    std::vector<Order> completedOrders;
    double rangesSum = 0.0; 
    // ASSERT(rangesSum == 360.0);
}
}
void ranges_ex2()
{
    test_1();
}