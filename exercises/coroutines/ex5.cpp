#include <coroutine>
#include <iostream>
#include <string>
#include <source_location>
#include <vector>

namespace {

// GetCurrentHandledoes not need changes, this is just a utility to get the current handle from within a coroutine
struct GetCurrentHandle {
    std::coroutine_handle<> h;
    bool await_ready() const noexcept { return false; } // Force suspension to get the handle
    bool await_suspend(std::coroutine_handle<> handle) noexcept {
        h = handle;
        return false; // Resume immediately, we just wanted the handle
    }
    std::coroutine_handle<> await_resume() const noexcept { return h; }
};

struct Task {
    struct promise_type {
        /*
        TODO: Implement a promise type that will
        A store some name, some source location info, and the parent handle (if any) for backtracing.
        the 'root' frame will nto have a parent.
        AFAIK you wont need more than these 3 fields to implement the backtrace logic, 
        but feel free to add more if you want to get creative with the output.
        */
        std::string name;
        std::source_location loc;
        std::coroutine_handle<> parent = nullptr;
    };

    //below is OK, no changes needed here to ctor or move ctor.
    std::coroutine_handle<promise_type> handle;
    Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    Task(Task&& other) noexcept : handle(other.handle) { other.handle = nullptr; }
    ~Task() { if (handle) handle.destroy(); }

    // This awaiter only exists to capture the handle and source location info for backtracing.
    // You can co_await this from within any coroutine to capture its handle and source location.
    // In practise youd want to launch the task on do the job in the same function, but for demonstration purposes we
    // want to be able to capture the handle from within the body of the coroutine.
    struct Awaiter {
        std::coroutine_handle<promise_type> child;
        std::source_location loc;

        bool await_ready() { return false; }
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> parent) {
            //TODO update the parent and loc here of child handle
            return child; 
        }
        void await_resume() {}
    };

    auto trace(std::source_location loc = std::source_location::current()) {
        return Awaiter{handle, loc};
    }
};

void dump_backtrace(std::coroutine_handle<> h) {
    std::cout << "\n--- LOGICAL BACKTRACE ---";
    //TODO walk over the coroutine_handle chain using the parent pointers, and print out the name and source location info for each frame.
    // Warning: This only works if every coroutine in the chain is a 'Task'
    std::cout << "\n-------------------------\n";
}

} // namespace

void coroutines_ex5() {
    auto leaf_coro = []() -> Task {
        // TODO: Before uncommenting implement the promise_type on Task.
        // TODO: Use our magic awaiter to grab the current handle, need to break out of the coroutine body
        // to get the handle to dump the trace
        
        // TODO:Now we can dump the trace from within the "leaf"
        //dump_backtrace(...);
        
        //co_return;
    };

    auto mid_coro = [&]() -> Task {
        /*
        TODO: Before uncommenting implement the promise_type on Task.

        auto t = leaf_coro();
        t.handle.promise().name = "LeafNode";
        co_await t.trace(); 
        co_return;
        */
    };

    auto root_coro = [&]() -> Task {
        /*
        TODO: Before uncommenting implement the promise_type on Task.

        auto t = mid_coro();
        t.handle.promise().name = "MidLevel";
        co_await t.trace();
        co_return;
        */
    };

    auto main_task = root_coro();
    main_task.handle.promise().name = "RootLevel";
    
    //TODO: start the chain.
    //uncomment once the trace logic is implemented. You should see a backtrace of RootLevel -> MidLevel -> LeafNode with source location info for each frame.
    //main_task.handle.resume();
}