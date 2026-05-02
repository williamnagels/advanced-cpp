#include <iostream>
#include <vector>
#include <cassert>

import Store; 

/*
GOAL:
1. Process simple Orders using the Store module.
2. Notice that Store::Order is visible because Store exported the :Data partition.
3. Trying building this. Youll get a build error complaining about store.ixx
4. Follow instruction on what to do in store.ixx to fix the build error.
*/
int main() {
    // Note: Store::Order comes from the partition :Data
    std::vector<Store::Order> orders = {
        {101, 150.0},
        {102, 200.0}
    };

    double total = Store::calculateTotal(orders);

    std::cout << "Order Count: " << orders.size() << "\n";
    std::cout << "Total Price: " << total << "\n";
    
    assert(total == 350.0);
    return 0;
}