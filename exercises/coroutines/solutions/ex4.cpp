#include <cassert>
#include <coroutine>
#include <iostream>
#include <memory>
#include <utility>

namespace
{
struct AndLatch {
    std::size_t counter;
    std::coroutine_handle<> parent;

    AndLatch(std::size_t count, std::coroutine_handle<> continuation)
        : counter(count), parent(continuation) {}

    ~AndLatch() { std::cout << "  [Debug] Latch destroyed\n"; }
};

struct Task {
    struct promise_type {
        std::shared_ptr<AndLatch> latch;

        Task get_return_object() noexcept {
            return Task{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(
                std::coroutine_handle<promise_type> handle) const noexcept {
                auto latch = handle.promise().latch;
                if (latch && --latch->counter == 0)
                    return latch->parent;
                return std::noop_coroutine();
            }
            void await_resume() const noexcept {}
        };

        FinalAwaiter final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    using handle_type = std::coroutine_handle<promise_type>;
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
};

struct WhenAll {
    Task& first;
    Task& second;

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<> parent) const {
        auto latch = std::make_shared<AndLatch>(2, parent);
        first.handle.promise().latch = latch;
        second.handle.promise().latch = std::move(latch);
        first.handle.resume();
        second.handle.resume();
    }

    void await_resume() const noexcept {}
};

void test_1()
{
    bool parent_resumed = false;
    int completed_workers = 0;

    auto worker = [&]() -> Task {
        ++completed_workers;
        std::cout << "Task\n";
        co_return;
    };

    auto parent = [&]() -> Task {
        auto first = worker();
        auto second = worker();

        std::cout << "Awaiting both tasks...\n";
        co_await WhenAll{first, second};

        parent_resumed = true;
        std::cout << "Resumed successfully!\n";
    };

    auto root = parent();
    root.handle.resume();

    assert(completed_workers == 2);
    assert(parent_resumed);
}
}

void coroutines_ex4()
{
    test_1();
}
