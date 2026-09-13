#include <algorithm>
#include <cassert>
#include <coroutine>
#include <iterator>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace
{
struct Messenger {
    struct promise_type {
        std::string value;

        Messenger get_return_object() noexcept {
            return Messenger{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() const noexcept {}
        std::suspend_always yield_value(std::string next_value) noexcept {
            value = std::move(next_value);
            return {};
        }
    };

    struct iterator {
        using iterator_concept = std::input_iterator_tag;
        using value_type = std::string;
        using difference_type = std::ptrdiff_t;

        std::coroutine_handle<promise_type> handle{};

        iterator& operator++() {
            handle.resume();
            return *this;
        }
        void operator++(int) { ++*this; }
        std::string const& operator*() const noexcept {
            return handle.promise().value;
        }
        friend bool operator==(
            iterator const& current, std::default_sentinel_t) noexcept {
            return !current.handle || current.handle.done();
        }
    };

    using handle_type = std::coroutine_handle<promise_type>;
    handle_type handle{};

    Messenger() = default;
    explicit Messenger(handle_type coroutine) noexcept : handle(coroutine) {}
    Messenger(Messenger const&) = delete;
    Messenger& operator=(Messenger const&) = delete;
    Messenger(Messenger&& other) noexcept
        : handle(std::exchange(other.handle, {})) {}
    Messenger& operator=(Messenger&&) = delete;

    iterator begin() {
        if (handle && !handle.done())
            handle.resume();
        return {handle};
    }
    std::default_sentinel_t end() const noexcept { return {}; }

    ~Messenger() {
        if (handle)
            handle.destroy();
    }
};

void test_1()
{
    auto producer = []() -> Messenger {
        co_yield "configure";
        co_yield "build";
        co_yield "test";
        co_yield "deploy";
    };

    auto stream = producer();
    std::vector<std::string> stages;
    std::ranges::copy(stream, std::back_inserter(stages));

    assert((stages == std::vector<std::string>{
        "configure", "build", "test", "deploy"}));
}
}

void coroutines_ex2()
{
    test_1();
}
