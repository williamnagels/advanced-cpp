#include <coroutine>
#include <iostream>
#include <cassert>

namespace {
/*
Task is OK no changes needed here
*/
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

struct Signal {
    std::coroutine_handle<> waiter;

    // TODO: Implement the 3 Awaiter methods:
    // 1. await_ready
    // 2. await_suspend (capture the handle)
    // 3. await_resume
    
    void fire() {
        if (waiter) waiter.resume();
    }
};
}
/*
GOAL:
Understand how co_await pauses execution and how resuming the handle 
from the OUTSIDE continues it. Using the signal as somekind of barrier
latch type of synchronization primitive in the coroutine
*/
void coroutines_ex2() {
    Signal sig;
    bool completed = false;

    auto my_coro = [&](Signal& s) -> Task {
        //co_await s; // TODO: uncomment once Signal is an awaitable
        completed = true;
    };

    my_coro(sig);
    assert(!completed); // Still waiting...
    
    sig.fire(); 
    assert(completed); // Now it's done!
}
