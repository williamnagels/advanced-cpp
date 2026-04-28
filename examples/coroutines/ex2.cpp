#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/detached.hpp>
#include <chrono>
#include <print>
#include <format>
#include <thread>
#include <sstream>
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;
using asio_timer = boost::asio::steady_timer;

std::string get_time() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    std::tm buf{};
    localtime_r(&time, &buf);
    return std::format("{:02}:{:02}:{:02}", buf.tm_hour, buf.tm_min, buf.tm_sec);
}
template<typename... Args>
void print(std::string_view fmt, Args&&... args) {
    std::string prefix = std::format("[{}] [thread {}] ", get_time(), std::this_thread::get_id());
    std::println("{}", prefix + std::vformat(fmt, std::make_format_args(args...)));
}
awaitable<void> timer_task(int id) {
    auto executor = co_await this_coro::executor;
    boost::asio::io_context& io =
        static_cast<boost::asio::io_context&>(executor.context());
    print("Task {} start", id);
    asio_timer t1(io, std::chrono::seconds(1));
    co_await t1.async_wait(use_awaitable);
    print("Task {} 1 second passed", id);
    asio_timer t2(io, std::chrono::seconds(2));
    co_await t2.async_wait(use_awaitable);
    print("Task {} 2 more seconds passed", id);
    print("Task {} done", id);
}

int main() {
    boost::asio::io_context io;
    co_spawn(io, timer_task(1), detached);
    co_spawn(io, timer_task(2), detached);
    co_spawn(io, timer_task(3), detached);
    print("Starting io_context with worker threads");
    std::vector<std::thread> workers;
    int num_threads = 2;
    for (int i = 0; i < num_threads; ++i) {
        workers.emplace_back([&io]{
            io.run();
        });
    }
    for (auto& w : workers)
        w.join();
    print("All tasks finished");
}
