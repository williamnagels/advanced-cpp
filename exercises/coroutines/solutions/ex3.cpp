#include <cassert>
#include <coroutine>
#include <exception>
#include <utility>

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

struct Signal {
    std::coroutine_handle<> waiter{};

    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> handle) noexcept {
        waiter = handle;
    }
    void await_resume() const noexcept {}

    void fire() {
        if (waiter) {
            auto handle = std::exchange(waiter, {});
            handle.resume();
        }
    }
};
}

void coroutines_ex3()
{
    Signal signal;
    bool completed = false;

    auto wait_for_signal = [&](Signal& awaited_signal) -> Task {
        co_await awaited_signal;
        completed = true;
    };

    wait_for_signal(signal);
    assert(!completed);

    signal.fire();
    assert(completed);
}
