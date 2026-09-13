#include <coroutine>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

namespace
{
struct Messenger {
    struct promise_type {
        std::string value;

        // TODO: Implement get_return_object, initial_suspend, final_suspend,
        // unhandled_exception, return_void, and yield_value.
    };

    struct iterator {
        using value_type = std::string;
        using difference_type = std::ptrdiff_t;

        std::coroutine_handle<promise_type> h{};

        // TODO: Implement prefix/postfix increment, dereference, and comparison
        // with std::default_sentinel_t.
    };

    std::coroutine_handle<promise_type> handle{};

    Messenger() = default;
    explicit Messenger(std::coroutine_handle<promise_type> coroutine) noexcept
        : handle(coroutine) {}
    Messenger(Messenger const&) = delete;
    Messenger& operator=(Messenger const&) = delete;
    Messenger(Messenger&& other) noexcept : handle(other.handle) {
        other.handle = {};
    }

    iterator begin() {
        if (handle && !handle.done())
            handle.resume();
        return {handle};
    }

    std::default_sentinel_t end() { return {}; }

    ~Messenger() {
        if (handle)
            handle.destroy();
    }
};

/*
GOAL:
Build a move-only coroutine generator that exposes lazily produced messages as
an input range.

1. Complete Messenger::promise_type. The generator must start suspended,
   suspend after each yielded value, and keep its frame alive at completion.
2. Complete Messenger::iterator so increment resumes the coroutine,
   dereference reads the promise value, and sentinel comparison detects
   completion.
3. Enable the deployment-status producer below.
4. Consume the generator with std::ranges::copy and std::back_inserter.

Observe that the producer retains its position between yields while the ranges
algorithm sees only an ordinary input range.
*/
void test_1()
{
    auto producer = []() -> Messenger {
        return {}; // TODO: Remove this placeholder when enabling co_yield.
        /*
        TODO: Uncomment once Messenger::promise_type is implemented.
        co_yield "configure";
        co_yield "build";
        co_yield "test";
        co_yield "deploy";
        */
    };

    auto stream = producer();
    std::vector<std::string> stages;

    // TODO: Copy every lazily produced stage into stages with
    // std::ranges::copy and std::back_inserter.

    // Uncomment once the exercise has been implemented.
    // assert((stages == std::vector<std::string>{
    //     "configure", "build", "test", "deploy"}));
}
}

void coroutines_ex2()
{
    test_1();
}