#include <array>
#include <cassert>
#include <condition_variable>
#include <coroutine>
#include <cstddef>
#include <deque>
#include <exception>
#include <latch>
#include <mutex>
#include <thread>
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

class ThreadPool {
public:
    struct ScheduleAwaiter {
        ThreadPool& pool;

        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> current) const {
            pool.enqueue(current);
        }
        void await_resume() const noexcept {}
    };

    explicit ThreadPool(std::size_t worker_count)
    {
        workers_.reserve(worker_count);
        for (std::size_t index = 0; index < worker_count; ++index)
            workers_.emplace_back([this] { worker_loop(); });
    }

    ~ThreadPool()
    {
        {
            std::lock_guard lock{mutex_};
            stopping_ = true;
        }
        work_ready_.notify_all();
        for (auto& worker : workers_)
            worker.join();
    }

    ScheduleAwaiter schedule() noexcept { return {*this}; }

    void wait_idle()
    {
        std::unique_lock lock{mutex_};
        idle_.wait(lock, [this] {
            return queue_.empty() && active_jobs_ == 0;
        });
    }

private:
    void enqueue(std::coroutine_handle<> handle)
    {
        {
            std::lock_guard lock{mutex_};
            queue_.push_back(handle);
        }
        work_ready_.notify_one();
    }

    void worker_loop()
    {
        while (true) {
            std::coroutine_handle<> next;
            {
                std::unique_lock lock{mutex_};
                work_ready_.wait(lock, [this] {
                    return stopping_ || !queue_.empty();
                });
                if (stopping_ && queue_.empty())
                    return;

                next = queue_.front();
                queue_.pop_front();
                ++active_jobs_;
            }

            next.resume();

            {
                std::lock_guard lock{mutex_};
                --active_jobs_;
                if (queue_.empty() && active_jobs_ == 0)
                    idle_.notify_all();
            }
        }
    }

    std::mutex mutex_;
    std::condition_variable work_ready_;
    std::condition_variable idle_;
    std::deque<std::coroutine_handle<>> queue_;
    std::size_t active_jobs_ = 0;
    bool stopping_ = false;
    std::vector<std::jthread> workers_;
};
}

void coroutines_ex7()
{
    ThreadPool pool{2};
    std::latch both_running{2};
    std::array<std::thread::id, 2> worker_ids{};
    std::array<bool, 2> completed{};

    auto worker = [&](std::size_t index) -> Task {
        co_await pool.schedule();
        worker_ids[index] = std::this_thread::get_id();
        both_running.arrive_and_wait();
        completed[index] = true;
    };

    auto first = worker(0);
    auto second = worker(1);
    pool.wait_idle();

    assert(completed[0] && completed[1]);
    assert(worker_ids[0] != std::thread::id{});
    assert(worker_ids[1] != std::thread::id{});
    assert(worker_ids[0] != worker_ids[1]);
}