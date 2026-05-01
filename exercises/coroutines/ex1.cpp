#include <coroutine>
#include <string>
#include <ranges>
#include <algorithm>
#include <cassert>

namespace {
struct Messenger {
    struct promise_type {
        std::string value;
        /*
        TODO: Implement:
            - get_return_object
            - initial_suspend
            - final_suspend
            - unhandled_exception
            - return_void
            - yield_value

        */
    };
    struct iterator {
        using value_type = std::string;
        using difference_type = std::ptrdiff_t;
        std::coroutine_handle<promise_type> h;
        /*
        TODO: Implement:
            - operator++ (both prefix and postfix)
            - operator*
            - operator== (to compare with std::default_sentinel_t)
        */
    };

    std::coroutine_handle<promise_type> handle;
    iterator begin() { if (handle) handle.resume(); return {handle}; }
    std::default_sentinel_t end() { return {}; }
    
    ~Messenger() { if (handle) handle.destroy(); }
};
}

void coroutines_ex1() {
    auto producer = []() -> Messenger {
        /*
        TODO: uncomment these once Messenger::promise_type is implementedd
        co_yield "Hello";
        co_yield "Ranges";
        co_yield "World";
        */
    };

    auto stream = producer();
    /*TODO: Select a ranges alogorithm and use the coroutine to create the string
    "Hello Ranges World"
    std::string result = <ranges algorithm> 

    assert(result == "Hello Ranges World");*/
}