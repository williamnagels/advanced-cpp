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

class ThreadPool {
public:
    struct ScheduleAwaiter {
        ThreadPool& pool;

        bool await_ready() const noexcept { return false; }

        void await_suspend(std::coroutine_handle<> current) const {
            // TODO: Enqueue current and wake one worker.
            (void)current;
        }

        void await_resume() const noexcept {}
    };

    explicit ThreadPool(std::size_t worker_count)
    {
        // TODO: Start worker_count jthreads that call worker_loop().
        (void)worker_count;
    }

    ~ThreadPool()
    {
        // TODO: Set stopping_ while holding mutex_, notify all workers, then
        // join them. Workers should drain already queued work before exiting.
    }

    ScheduleAwaiter schedule() noexcept { return {*this}; }

    void wait_idle()
    {
        // TODO: Wait until the queue is empty and active_jobs_ is zero.
    }

private:
    void enqueue(std::coroutine_handle<> handle)
    {
        // TODO: Push handle under mutex_, then notify one worker.
        (void)handle;
    }

    void worker_loop()
    {
        // TODO: Repeatedly wait for work or shutdown. Pop one handle and
        // increment active_jobs_ under the lock, but call resume() only after
        // releasing the lock. Afterwards decrement active_jobs_ and notify
        // idle_ when no work remains.
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

/*
GOAL:
Turn exercise 5's manual FIFO executor into a thread-safe executor with two
worker threads. Synchronize the queue, never resume a coroutine while holding
the queue mutex, and wait for all resumed coroutines before destroying their
frames.
*/
void coroutines_ex7()
{
    // TODO: Enable this test after implementing ThreadPool.
    /*
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
    */
}