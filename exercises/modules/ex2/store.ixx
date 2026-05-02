module;
#include <vector>

export module Store;
export import :Data; // Combine the partition into the primary module

export namespace Store {
    double calculateTotal(const std::vector<Order>& orders);
}
// After seeing the build error:
// Trying to hide the implementation of calculateTotal from users of the Store module
// so a change wont trigger rebuild.
// Base idea: This is a private partition, so its contents are not visible outside the module.
// Reimplement this with an module implementation file (StoreImpl.ixx) 
// see how it affects the build when you change the implementation of calculateTotal.
module :private; 

namespace Store {
    double calculateTotal(const std::vector<Order>& orders) {
        double total = 0;
        for (const auto& o : orders) {
            total += o.totalPrice;
        }
        return total;
    }
}