#include <cassert>
#include <coroutine>
#include <cstddef>
#include <deque>
#include <exception>
#include <utility>
#include <vector>

namespace
{
// Task owns its coroutine frame. No changes are needed here.
struct Task {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        Task get_return_object() noexcept {
            return Task{handle_type::from_promise(*this)};
        }
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
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
};

class ManualExecutor {
public:
    struct ScheduleAwaitable {
        ManualExecutor& executor;

        struct Awaiter {
            ManualExecutor& executor;

            // TODO: Scheduling must always take the suspension path.
            bool await_ready() const noexcept { return true; }

            // TODO: Enqueue current instead of resuming it here.
            void await_suspend(std::coroutine_handle<> current) const {
                (void)current;
            }

            // TODO: Return the executor's current resumption number.
            std::size_t await_resume() const noexcept { return 0; }
        };

        // TODO: Return an Awaiter connected to executor.
        Awaiter operator co_await() const noexcept { return {executor}; }
    };

    ScheduleAwaitable schedule() noexcept { return {*this}; }

    bool run_one()
    {
        // TODO: If the queue is empty, return false. Otherwise remove its
        // first handle, increment resume_count_, resume it, and return true.
        return false;
    }

    std::size_t pending() const noexcept { return queue_.size(); }

private:
    std::deque<std::coroutine_handle<>> queue_;
    std::size_t resume_count_ = 0;
};
}

/*
GOAL:
Build a tiny FIFO executor and expose scheduling through a separate awaitable
and awaiter. Observe that await_suspend() can hand a coroutine handle to a
runtime instead of resuming it immediately, and that await_resume() supplies
the value of the co_await expression.
*/
void coroutines_ex5()
{
    ManualExecutor executor;
    std::vector<int> execution_order;
    std::vector<std::size_t> resume_numbers;

    auto worker = [&](int id) -> Task {
        // TODO: Enable this after implementing ScheduleAwaitable::Awaiter.
        // auto resume_number = co_await executor.schedule();
        // execution_order.push_back(id);
        // resume_numbers.push_back(resume_number);
        (void)id;
        co_return;
    };

    auto first = worker(10);
    auto second = worker(20);

    // TODO: Enable these assertions. Both tasks should be queued, not finished.
    // assert(executor.pending() == 2);
    // assert(execution_order.empty());

    // assert(executor.run_one());
    // assert((execution_order == std::vector{10}));
    // assert((resume_numbers == std::vector<std::size_t>{1}));

    // assert(executor.run_one());
    // assert((execution_order == std::vector{10, 20}));
    // assert((resume_numbers == std::vector<std::size_t>{1, 2}));
    // assert(!executor.run_one());

    (void)first;
    (void)second;
    (void)execution_order;
    (void)resume_numbers;
}