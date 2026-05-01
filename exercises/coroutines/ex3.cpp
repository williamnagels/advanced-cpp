#include <coroutine>
#include <cassert>
#include <exception>

namespace {

struct Task {
    struct promise_type {
        Task get_return_object() { return {std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> handle;
};
struct Switchboard {
    int input_val;

    // TODO: Implement await_ready (return false to suspend)
    // TODO: Implement await_suspend (resume the handle immediately)
    // TODO: Implement await_resume (return input_val * 2)
};
}

/*
GOAL:
Understand that co_await is an expression that returns a value.
The value returned by await_resume() is what the coroutine receives.
Awaiters are typically used to break out of code where you dont have access to the handle
(body of math_coro) to a place of code where the handle is know.

*/
void coroutines_ex3() {
    int final_result = 0;

    auto math_coro = [&](int start) -> Task {
        // TODO uncomment once switchboard is an awaitable
        // int doubled = co_await Switchboard{start}; 
        //final_result = doubled;
    };

    math_coro(10);
    // assert(final_result == 20);
}