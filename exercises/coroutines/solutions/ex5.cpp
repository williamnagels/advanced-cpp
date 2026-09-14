#include <cassert>
#include <coroutine>
#include <cstddef>
#include <deque>
#include <exception>
#include <utility>
#include <vector>

namespace
{
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

            bool await_ready() const noexcept { return false; }
            void await_suspend(std::coroutine_handle<> current) const {
                executor.queue_.push_back(current);
            }
            std::size_t await_resume() const noexcept {
                return executor.resume_count_;
            }
        };

        Awaiter operator co_await() const noexcept { return {executor}; }
    };

    ScheduleAwaitable schedule() noexcept { return {*this}; }

    bool run_one()
    {
        if (queue_.empty())
            return false;

        auto next = queue_.front();
        queue_.pop_front();
        ++resume_count_;
        next.resume();
        return true;
    }

    std::size_t pending() const noexcept { return queue_.size(); }

private:
    std::deque<std::coroutine_handle<>> queue_;
    std::size_t resume_count_ = 0;
};

}

void coroutines_ex5()
{
    ManualExecutor executor;
    std::vector<int> execution_order;
    std::vector<std::size_t> resume_numbers;

    auto worker = [&](int id) -> Task {
        auto resume_number = co_await executor.schedule();
        execution_order.push_back(id);
        resume_numbers.push_back(resume_number);
    };

    auto first = worker(10);
    auto second = worker(20);

    assert(executor.pending() == 2);
    assert(execution_order.empty());

    assert(executor.run_one());
    assert((execution_order == std::vector{10}));
    assert((resume_numbers == std::vector<std::size_t>{1}));

    assert(executor.run_one());
    assert((execution_order == std::vector{10, 20}));
    assert((resume_numbers == std::vector<std::size_t>{1, 2}));
    assert(!executor.run_one());
}
