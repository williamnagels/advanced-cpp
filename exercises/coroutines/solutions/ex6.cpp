#include <algorithm>
#include <coroutine>
#include <iostream>
#include <source_location>
#include <string>
#include <utility>
#include <vector>

namespace
{
struct GetCurrentHandle {
    std::coroutine_handle<> handle{};

    bool await_ready() const noexcept { return false; }
    bool await_suspend(std::coroutine_handle<> current) noexcept {
        handle = current;
        return false;
    }
    std::coroutine_handle<> await_resume() const noexcept { return handle; }
};

struct Task {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        std::string name;
        std::source_location location = std::source_location::current();
        std::coroutine_handle<> parent{};

        Task get_return_object() noexcept {
            return Task{handle_type::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(handle_type current) const noexcept {
                auto parent = current.promise().parent;
                return parent ? parent : std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };

        FinalAwaiter final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    handle_type handle{};

    explicit Task(handle_type coroutine) noexcept : handle(coroutine) {}
    Task(Task const&) = delete;
    Task& operator=(Task const&) = delete;
    Task(Task&& other) noexcept
        : handle(std::exchange(other.handle, {})) {}
    Task& operator=(Task&&) = delete;

    ~Task() {
        if (handle)
            handle.destroy();
    }

    struct Awaiter {
        handle_type child;
        std::source_location location;

        bool await_ready() const noexcept { return child.done(); }
        std::coroutine_handle<> await_suspend(
            std::coroutine_handle<> parent) const noexcept {
            child.promise().parent = parent;
            child.promise().location = location;
            return child;
        }
        void await_resume() const noexcept {}
    };

    Awaiter trace(
        std::source_location location = std::source_location::current()) noexcept {
        return {handle, location};
    }
};

void dump_backtrace(std::coroutine_handle<> current)
{
    std::vector<Task::handle_type> frames;
    while (current) {
        auto frame = Task::handle_type::from_address(current.address());
        frames.push_back(frame);
        current = frame.promise().parent;
    }

    std::cout << "\n--- LOGICAL BACKTRACE ---\n";
    for (auto frame = frames.rbegin(); frame != frames.rend(); ++frame) {
        auto const& promise = (*frame).promise();
        std::cout << promise.name << " at "
                  << promise.location.file_name() << ':'
                  << promise.location.line() << '\n';
    }
    std::cout << "-------------------------\n";
}
}

void coroutines_ex6()
{
    auto leaf_coro = []() -> Task {
        auto current = co_await GetCurrentHandle{};
        dump_backtrace(current);
    };

    auto mid_coro = [&]() -> Task {
        auto leaf = leaf_coro();
        leaf.handle.promise().name = "LeafNode";
        co_await leaf.trace();
    };

    auto root_coro = [&]() -> Task {
        auto middle = mid_coro();
        middle.handle.promise().name = "MidLevel";
        co_await middle.trace();
    };

    auto root = root_coro();
    root.handle.promise().name = "RootLevel";
    root.handle.promise().location = std::source_location::current();
    root.handle.resume();
}
