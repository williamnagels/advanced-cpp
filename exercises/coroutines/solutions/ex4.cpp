#include <cassert>
#include <coroutine>
#include <exception>

namespace
{
struct Task {
    struct promise_type {
        Task get_return_object() const noexcept { return {}; }
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_never final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
};

struct Switchboard {
    int input_value;

    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> handle) const noexcept {
        handle.resume();
    }
    int await_resume() const noexcept { return input_value * 2; }
};
}

void coroutines_ex4()
{
    int final_result = 0;

    auto calculate = [&](int start) -> Task {
        int doubled = co_await Switchboard{start};
        final_result = doubled;
    };

    calculate(10);
    assert(final_result == 20);
}
