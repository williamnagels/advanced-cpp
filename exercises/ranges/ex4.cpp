#include <ranges>
#include <vector>
#include <span>
#include <cassert>
#include <iostream>

namespace 
{
/*
TODO: constrain V with a sensible range for a sliding forward window
template< CONSTRAIN_ME V>
class sliding_window_view : public std::ranges::view_interface<sliding_window_view<V>> {
    // TODO: add base range and a param window_size (size of the window - used to slide over base range)
public:
    // TODO:add constructors

    struct iterator {
        // below is the standard boilerplate for a forward iterator, 
        // but the value_type is a subrange representing the current window
        // no changes needed to the iterator category, difference_type, pointer, or reference
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::ranges::subrange<std::ranges::iterator_t<const V>>;
        using difference_type = std::ranges::range_difference_t<V>;
        using pointer = void;
        using reference = value_type;

        // Returns a subrange representing the current window
        value_type operator*() const {
            //TODO implement
        }

        iterator& operator++() {
            //TODO implement
        }
        iterator operator++(int) {
            //TODO implement
        }
        bool operator==(const iterator& other) const = default;
        bool operator==(std::ranges::sentinel_t<const V> s) const {
            // We can't form a full window if there are fewer than size elements left
            // TODO implement
        }
    };

    auto begin() //TODO: should this be const?
    {
        // TODO implement

    }
    auto end() //TODO: should this be const?
    { 
        // TODO implement
    }
};*/

// TODO:A sliding_window_view is "borrowed" ONLY if the underlying range is borrowed.
// This is a conditional specialization!
// Is this in the correct namespace in this cpp file? why not?
/*
template<typename V>
inline constexpr bool std::ranges::enable_borrowed_range<...> = .... */

void test_sliding_window() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    // 1. Create a window view over a span (which is a borrowed range)
    std::span s{vec};
   
    /* 
    TODO: uncomment when sliding_window_view is implemented

    auto windows = sliding_window_view(std::views::all(s), 2);
    auto it = std::ranges::find_if(std::move(windows), [](auto window) {
        return *window.begin() == 2;
    });

    static_assert(!std::same_as<decltype(it), std::ranges::dangling>, 
                  "Error: Should be borrowed because the underlying span is borrowed!");

    assert(*(*it).begin() == 2);
    assert(*std::next((*it).begin()) == 3);
    */
}

void test_dangling_window() {
    
    //TODO: uncomment assert and code below when dangling iterator is returned
    /* 
    auto generateDanglingVec = []() { return std::vector{1, 2, 3}; };
    // windows is now an owning_view (vector returned by r-value)
    auto windows = sliding_window_view(std::views::all(generateDanglingVec()), 2);
    auto it = std::ranges::find_if(std::move(windows), [](auto) { return true; });
    static_assert(std::same_as<decltype(it), std::ranges::dangling>, 
                  "Error: Should dangle because the underlying vector is temporary!");
    */
}
}
/*
GOAL:
The goal of this exercise is to implement a 'sliding_window_view'—a view that 
produces "subranges" of size N, sliding one element at a time.

1. Constraints: The view must work with at least 'std::ranges::forward_range' 
   because we need to look ahead without consuming the range.
2. The Iterator: The iterator's value_type is a 'std::ranges::subrange'. 
   Think of the iterator as holding the 'begin' and 'end' of the current window.
3. The Sentinel: A window cannot be formed if there are fewer than 'N' 
   elements left. Your sentinel comparison must handle this logic.
4. Conditional Borrowing: This is the core of the exercise. Specialize 
   'enable_borrowed_range' so that the view is only considered "borrowed" 
   if the underlying base range is also borrowed.
*/
void ranges_ex4() {
    test_sliding_window();
    test_dangling_window();
}