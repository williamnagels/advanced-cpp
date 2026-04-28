#include <iostream>
#include <vector>
#include <ranges>     // Ranges support
#include <coroutine>  // Coroutine support
#include <expected>   // C++23 specific feature

// A simple C++23 test: using std::expected (New in C++23)
std::expected<int, std::string> check_cpp23_support(bool success) {
    if (success) return 42;
    return std::unexpected("C++23 Expected feature failed");
}

// Minimal Coroutine Task type for testing
struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

Task test_coroutine() {
    co_return; 
}

int main() {
    // 1. Test Ranges (C++20/23)
    std::vector<int> nums = {1, 2, 3, 4, 5};
    auto view = nums | std::views::filter([](int n) { return n % 2 == 0; });

    std::cout << "Testing C++23 Features..." << std::endl;

    // 2. Test std::expected (C++23)
    auto result = check_cpp23_support(true);
    if (result) {
        std::cout << "Success: C++23 std::expected is available!" << std::endl;
    }

    // 3. Test Ranges result
    std::cout << "Ranges test (even numbers): ";
    for (int n : view) std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}