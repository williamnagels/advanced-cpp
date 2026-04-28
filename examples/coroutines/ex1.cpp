#include <coroutine>
#include <chrono>
#include <sys/timerfd.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <print>

class EventLoop;

// A single global loop for simplicity
EventLoop& getLoop();
std::string getcurtime()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto t   = floor<milliseconds>(now);

    return std::format("{:%H:%M:%S}.{:03}",
                       floor<seconds>(t),
                       (t.time_since_epoch() % seconds{1}) / milliseconds{1});
}
// ---------------- Awaitable Timer ----------------
class Timer {
public:
    explicit Timer(std::chrono::milliseconds dur)
        : duration(dur) {}

    bool await_ready() const noexcept {
        return duration.count() == 0;
    }

    void await_suspend(std::coroutine_handle<> h);

    void await_resume() const noexcept {}

private:
    std::chrono::milliseconds duration;
};

// ---------------- Simple Task Type ----------------
struct Task {
    struct promise_type {
        Task get_return_object() {
            return Task{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() {}
    };

    std::coroutine_handle<promise_type> handle;
};

// ---------------- Event Loop ----------------
class EventLoop {
public:
    EventLoop() {
        epfd = epoll_create1(0);
        if (epfd < 0) {
            perror("epoll_create1");
            std::exit(1);
        }
    }

    ~EventLoop() { close(epfd); }

    void addTimer(int fd, std::coroutine_handle<> h) {
        epoll_event ev{};
        ev.events = EPOLLIN;
        ev.data.fd = fd;

        if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) < 0) {
            perror("epoll_ctl");
            std::exit(1);
        }
        waiters.push_back({fd, h});
    }

    void run() {
        while (!waiters.empty()) {
            epoll_event ev{};
            int n = epoll_wait(epfd, &ev, 1, -1);
            if (n <= 0) continue;

            // find the waiter
            for (auto it = waiters.begin(); it != waiters.end(); ++it) {
                if (it->fd == ev.data.fd) {
                    // must read timerfd to clear it
                    uint64_t exp;
                    read(it->fd, &exp, sizeof(exp));
                    close(it->fd);
                    auto coro = it->h;
                    waiters.erase(it);
                    coro.resume();
                    break;
                }
            }
        }
    }

private:
    struct Waiter {
        int fd;
        std::coroutine_handle<> h;
    };

    int epfd;
    std::vector<Waiter> waiters;
};

EventLoop& getLoop() { static EventLoop loop; return loop; }

// ---------------- Timer await_suspend ----------------
void Timer::await_suspend(std::coroutine_handle<> h) {
    int fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (fd < 0) {
        perror("timerfd_create");
        std::exit(1);
    }

    itimerspec spec{};
    spec.it_value.tv_sec = duration.count() / 1000;
    spec.it_value.tv_nsec = (duration.count() % 1000) * 1'000'000;

    if (timerfd_settime(fd, 0, &spec, nullptr) < 0) {
        perror("timerfd_settime");
        std::exit(1);
    }

    getLoop().addTimer(fd, h);
}

Task example() {
    std::println("{}:Waiting 1s",getcurtime());
    co_await Timer{std::chrono::seconds(1)};
    std::println("{}:Done!",getcurtime());

    std::println("{}:Waiting 500 ms",getcurtime());
    co_await Timer{std::chrono::milliseconds(500)};
    std::println("{}:Done!",getcurtime());
}
int main() {
    example();
    getLoop().run();
}
