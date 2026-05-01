#include <coroutine>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

namespace {

// The shared state managed by tasks and the WhenAll awaiter
// AndLatch code is OK, doesnt need changes
struct AndLatch 
{
    std::size_t counter;
    std::coroutine_handle<> parent;

    AndLatch(std::size_t n, std::coroutine_handle<> p) 
        : counter(n), parent(p) {}
    
    ~AndLatch() { std::cout << "  [Debug] Latch destroyed\n"; }
};

struct Task {
    struct promise_type {
        // TODO: Each task holds a managed ptr to the synchronization logic
        // Add this here.

        Task get_return_object() { 
            return {std::coroutine_handle<promise_type>::from_promise(*this)}; 
        }
        std::suspend_always initial_suspend() { return {}; }
        
        struct FinalAwaiter {
            /*
            Implement final awaiter here.
            Look up the shared state from the promise, decrement the counter, and if it hits zero, resume the parent.
            */
            bool await_ready() noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept {
                return std::noop_coroutine();
            }
            void await_resume() noexcept {}
        };

        // When a task is completed, the final awaiter
        // is constructed.
        FinalAwaiter final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> handle;
    Task(std::coroutine_handle<promise_type> h) : handle(h) {}

    ~Task() { if (handle) handle.destroy(); }
    Task(const Task&) = delete;
    Task(Task&& other) noexcept : handle(other.handle) { other.handle = nullptr; }
};

struct WhenAll {
    Task& t1;
    Task& t2;

    bool await_ready() noexcept { return false; }

    void await_suspend(std::coroutine_handle<> parent_handle) {
        // Initialize the shared state with a count of 2 OR
        // use the managed ptr to track when the continuation should be ran
        // remember, parent_handle is the continuation. it is running
        // this function to ask when im being suspended (right now) what
        // should I do?
    }

    void await_resume() noexcept {}
};

/* 
GOAL: 
Validate the AND logic using smart ptr for automatic memory management 
of the synchronization primitive.
*/
void test_1() {

    //TODO: uncomment once the final awaiter is implemented. This should start the chain of execution.
    // Verify both Task prints (in coro worker) are printed to std::cout
    /*
    bool parent_resumed = false;

    auto worker = []() -> Task {
         std::cout << "Task" << std::endl;
        co_return; 
    };

    auto main_coro = [&]() -> Task {
        auto t1 = worker();
        auto t2 = worker();
        
        std::cout << "Awaiting both tasks..." << std::endl;;
        co_await WhenAll{t1, t2};
        
        parent_resumed = true;
        std::cout << "Resumed successfully!" << std::endl;;
        co_return;
    };

    auto root = main_coro();
    root.handle.resume();

    assert(parent_resumed);
    */
}
}

void coroutines_ex4() {
    test_1();
}