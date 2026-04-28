#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cassert>
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

1. Filter the 'orders' vector: Copy only orders where the status is "Completed" 
   into the 'completedOrders' vector.
2. Calculate the Sum: Use a numeric algorithm to sum the 'totalPrice' field 
   of all orders now residing in 'completedOrders'.

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
    double stlSum = 0.0;
    
    //Uncomment assert once exersise has been completed
    //assert(stlSum == 360.0);
}
}
void ranges_ex1()
{
    test_1();
}
