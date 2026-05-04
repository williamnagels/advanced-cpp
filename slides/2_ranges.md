---
marp: true
theme: slide-theme
---
<!-- _class: first-slide -->
---
# Ranges
<!-- _class: second-slide -->
---
# Ranges
- Why do we need ranges?
- What is a range really?
- views and adaptors
- Sentinels
---
# Why do we need ranges?
## Iterators are fundamentally unsafe
There are no built-in safety checks or sentinels to prevent their use after invalidation
```cpp
std::vector<int> v{1, 2, 3};
auto it = v.begin();
v.push_back(4);   // may reallocate, invalidates iterators
int x = *it;      // undefined behaviour
```
---
## Iterators and sentinels
Iterators always come in pairs of the same type: begin and end.
This makes some patterns awkward (e.g., searching until a terminator).
```cpp
// Can't stop at '\0' without computing std::end(...)
for (auto it = std::begin(arr); it != std::end(arr); ++it) {
    ...
}
```
We need to iterate until end but the stop condition is until we find the terminator.

---
## Composability
Iterators don’t compose well.
You typically end up writing loops instead of combining operations.
```cpp
std::vector<int> result;
for (auto it = v.begin(); it != v.end(); ++it) {
    if (!predicate(*it)) continue;
    auto transformed = transform(*it);
    if (stop_condition(transformed)) break;
    result.push_back(transformed);
}
```
---

Standard STL algorithms are **eager**. They process the entire range immediately, producing a full result before the next step begins.

- Large-scale datasets that don't fit in RAM.
- Real-time data streams where you don't know the end.
    - Custom iterators

---
## How to handle failure?

If std::find fails to find anything, you must manually compare with end().

- Dereferencing the end() iterator is UB.
- Compiler does not help you here.

Example
```cpp
auto it = std::find(v.begin(), v.end(), 99);
int x = *it;   // BUG if element not found
```
---
## Iterator Invalidation
Different containers have different invalidation rules.
| Container     | push_back invalidates? |
| ------------- | ---------------------- |
| `std::vector` | Possibly               |
| `std::list`   | Never                  |

---

ex1.cpp:
Convert a raw for loop to an STL algorithm.
(re)discover the pain of using iterators.

---
# Ranges
## What is a range?
A range is any object that can produce a begin and an end.
```cpp
template< class T >
concept range = requires( T& t ) {
    ranges::begin(t);
    ranges::end (t);
};
```
---
## Goal?
- Make iteration explicit and unified
- Improve safety and clarity
- Reduce manual iterator handling
- Allow algorithms to work directly on containers
- Lazy access to the underlying container
```cpp
std::sort(v.begin(), v.end());
std::sort(v);
```
---
## Templates
Most of the intelligent things happen at compile time.
```cpp
template< ranges::random_access_range R, class Comp = ranges::less,
          class Proj = std::identity >
requires std::sortable<ranges::iterator_t<R>, Comp, Proj>
constexpr ranges::borrowed_iterator_t<R>
    sort( R&& r, Comp comp = {}, Proj proj = {} );
```
We already know this concept more or less:
```cpp
ranges::random_access_range R
```

---
## Projection: Identity
Remove boilerplate using projection
Increase expressiveness
```cpp
struct Lad {
    std::string name;
    int age;
};
int main() {
    std::vector<Lad> theLads = { {"Erik", 77}, {"Bob", 33}, {"Charlie", 53} };
    std::ranges::sort(theLads, 
        [](const auto& a, const auto& b) { return a.age > b.age; });
    std::ranges::sort(theLads, std::ranges::greater{}, &Lad::age);
}-
```
---
## Projection
- Transformed by projection then the comparator evaluation.
- Invokable: Projections can be member pointers, function pointers, or lambdas.
- Performance: Projections are called twice per comparison (once for each side).
---
## Ranges: sentinel
Overload operator== for the sentinel
```cpp
bool operator==(const char* it, semicolon_sentinel)
{
    return *it == ';';
}

const char* text = "abc;def";
auto r = std::ranges::subrange(
    text,                  // iterator
    semicolon_sentinel{}   // sentinel
);
```
---
## View
A view is a type that is:
- Definitly a range
- Cheap to move
- Possibly owns the container
```cpp
template<class T>
concept view = ranges::range<T> && std::movable<T> && ranges::enable_view<T>;

auto get_first_three_view(std::vector<int>& v) {
    return std::ranges::subrange(v.begin(), v.begin() + 3);   // view
}
```
---
The concept of views was invented for composability:
- range → transform → filter → slice → consume
- Do not want to copy underlying data when passing into the next algorithm
- The view is copied, not the underlying container
---
```cpp
std::vector<int> v{1, 2, 3, 4, 5, 6};
auto r1 = std::ranges::subrange(v);
auto r2 = std::ranges::take_view(r1, 4);
auto r3 = std::ranges::filter_view(r2,
    [](int x){ return x % 2 == 0; });
for (int x : r3)
    std::cout << x << " ";
```
---
## Pipeline syntax
- Use pipe '|' symbol
- Temporary views are created that do not outlive the expression
```cpp
std::vector<int> v{1, 2, 3, 4, 5, 6};

//type of r is a view
auto r = v 
        | std::ranges::views::take(4)
        | std::ranges::views::filter([](int x){ return x % 2 == 0; });

for (int x : r)
    std::cout << x << " ";
```
---
# Adaptors
- Adaptors are factories for views.
- The filter adapter generates the filter view.

Examples:
- std::views::filter
- std::views::transform
- std::views::take

---
## Ranges are complicated types
What is the real type?
```cpp
#include <ranges>
#include <vector>
#include <iostream>
std::string demangle(const char* name);
int main()
{
    std::vector<int> v = {1,2,3,4};

    auto is_even = [](int x){ return x % 2 == 0; };
    std::ranges::filter_view fv(v, is_even);
    std::cout << demangle(typeid(fv).name()) << std::endl;
}
```
---
```cpp
std::string demangle(const char* name)
{
    int status = 0;
    std::unique_ptr<char, void(*)(void*)> res{
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    };
    return (status == 0) ? res.get() : name;
}
```
```cpp
std::ranges::filter_view<std::ranges::ref_view<std::vector<int, std::allocator<int> > >, main::{lambda(int)#1}>
```
- Filter predicate is part of the type? Good --> EBO
---
Only possible because of Class Template Argument Deduction (CTAD, C++17) .

Not possible as return value. rv must be fully qualified (or deduced).
```cpp
template< ranges::input_range V,
          std::indirect_unary_predicate<ranges::iterator_t<V>> Pred >
    requires ranges::view<V> && std::is_object_v<Pred>
class filter_view
    : public ranges::view_interface<filter_view<V, Pred>>

```

---
The range is wrapped in a ref_view by the adaptor to ensure a copy is lightweight.
Can fully qualify.
```cpp
std::vector<int> v = {1,2,3,4};
auto is_even = [](int x){ return x % 2 == 0; };
auto factory = std::views::filter(is_even);
auto view = v | factory;
static_assert(
    std::same_as<
        decltype(view),
        std::ranges::filter_view<std::ranges::ref_view<decltype(v)>, decltype(is_even)>
    >
);
```
---
Views try to be cheap to copy but dont have to be
```cpp
int main()
{
    std::vector<int> v = {1, 2, 3};
    // l-value case
    auto view1 = std::views::all(v);
    static_assert(
        std::is_same_v<decltype(view1), std::ranges::ref_view<std::vector<int>>>,
        "Expected ref_view for lvalue"
    );
    // r-value case
    auto view2 = std::views::all(std::vector<int>{1, 2, 3});
    static_assert(
        std::is_same_v<decltype(view2), std::ranges::owning_view<std::vector<int>>>,
        "Expected owning_view for rvalue"
    );
    return 0;
}
```
---
rewrite ex1 using ranges.
ex2.cpp

---
## Custom views and adaptors
Create a moving average algorithm. Given a range of numbers A. Produce a new range B with the moving average of the numbers in A.
need some kind of stateful iterator, not provided by std
```cpp
int main() {
    std::vector<double> data = {1, 2, 3, 4, 5, 6};
    auto avg3 = data | moving_average(3);
    return 0;
}
```
---
Define a struct 'moving_average'
Concept used to constrain templated parameter r
This is the adaptor, the view factory.
```cpp
struct moving_average {
    std::size_t window_;
    friend auto operator|(std::ranges::viewable_range auto&& r, moving_average const& self) {
        return moving_average_view(std::forward<decltype(r)>(r), self.window_);
    }
};
```
---
```cpp
template <std::ranges::input_range V>
requires std::ranges::view<V> && std::is_arithmetic_v<std::ranges::range_value_t<V>>
class moving_average_view
    : public std::ranges::view_interface<moving_average_view<V>>
{
    V base_;
    std::size_t window_;
    using T = std::ranges::range_value_t<V>;
public:
    template <std::ranges::viewable_range R>
    moving_average_view(R&& r, std::size_t w)
        : base_(std::forward<R>(r)), window_(w) {}
    struct iterator {
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::input_iterator_tag;
        std::deque<T> buf_;
        iterator() = default;
        iterator(std::ranges::iterator_t<V> it,
                 std::ranges::sentinel_t<V> end,
                 std::size_t window)
            : it_(it), end_(end), window_(window) {
        }
        value_type operator*() const {
        }
        iterator& operator++() {
        }
    };
    iterator begin() {
        return iterator(std::ranges::begin(base_), std::ranges::end(base_), window_);
    }
    std::default_sentinel_t end() const noexcept {
        return {};
    }
};

```
---
## The power of std::views::all_t
CTAD rule, deduce parameter V from parameter R.
```cpp
template <class R>
moving_average_view(R&&, std::size_t)
-> moving_average_view<std::views::all_t<R>>;
```
std::views::all_t deduces to ref_view or owning_view depending on R.
```cpp
int main() {
    std::vector<double> data = {1, 2, 3, 4, 5, 6};
    auto avg3 = data | moving_average(3);
    static_assert(std::same_as<decltype(avg3), moving_average_view<std::ranges::ref_view<std::vector<double>>>>);
    auto avg4 = std::move(data) | moving_average(3);
    static_assert(std::same_as<decltype(avg4), moving_average_view<std::ranges::owning_view<std::vector<double>>>>);
    return 0;
}
```
---
Adapter closure now only works in pipeline
Free pipeline operator --> inherit from std::ranges::range_adaptor_closure

```cpp
struct moving_average : std::ranges::range_adaptor_closure<moving_average>
{
    std::size_t window_;
    auto operator()(std::ranges::viewable_range auto&& r) const {
        return moving_average_view(std::forward<decltype(r)>(r), window_);
    }
};
int main() {
    std::vector<double> data = {1,2,3,4,5,6};
    auto avg_pipe = data | moving_average{.window_ = 3};
    static_assert(std::same_as<decltype(avg_pipe), moving_average_view<std::ranges::ref_view<std::vector<double>>>>);
    moving_average ma{.window_ = 3};       // closure object
    auto avg3 = ma(data);                  // call operator() directly
    static_assert(std::same_as<decltype(avg3), moving_average_view<std::ranges::ref_view<std::vector<double>>>>);
}

```
---
ex3.cpp

---
C++ Ranges have a "Type-Safe Tombstone"
```cpp
auto get_data() { return std::vector{1, 2, 3}; }
int main() 
{
    // std::ranges::find returns std::ranges::dangling.
    auto it = std::ranges::find(get_data(), 2);
    return *it; 
}
```
---
Runtime UB error intercepted by the compiler:
```
<source>: In function 'int main()':
<source>:9:12: error: no match for 'operator*' (operand type is 'std::ranges::dangling')
    9 |     return *it;
      |            ^~~
Compiler returned: 1
```
---
Why does std::find return dangling while views::all returns an owning_view?
- Algorithms return iterators. They cannot own the container.
- Views are objects. When given an Rvalue, they move the data into an owning_view.
```cpp
//moving_average_view<std::ranges::owning_view<std::vector<int>>>
auto v = get_data() | std::views::filter(is_even);
// 'it' cannot hold the vector. To prevent a crash, it becomes 'dangling'.
auto it = std::ranges::find(get_data(), 2);
```
---
## Borrowed ranges
Observation: Not all r-value ranges are unsafe
If the range dies, the iterators remain valid
```cpp
auto it = std::ranges::find(std::string_view{"Hello"}, 'e');
static_assert(!std::same_as<decltype(it), std::ranges::dangling>);
```
---
```cpp
template<typename T>
struct MyCursor {
    T* ptr;
    T* begin() const { return ptr; }
    T* end() const { return ptr + 10; }
};
template<typename T>
inline constexpr bool std::ranges::enable_borrowed_range<MyCursor<T>> = true;
int main() {
    auto it = std::ranges::find(MyCursor<int>{nullptr}, 42); 
    return *it;
}
```
---
```cpp
template<std::ranges::input_range R>
decltype(auto) get_first(R&& r) {
    if constexpr (std::ranges::borrowed_range<R>) {
        return *std::ranges::begin(r); // Returns Reference (Fast)
    } else {
        using V = std::ranges::range_value_t<R>;
        return static_cast<V>(*std::ranges::begin(r)); // Returns Copy (Safe)
    }
}
int main() {
    decltype(auto) val = get_first(std::vector{1, 2});
}
```
---
Custom sentinel: keeps iterating until std::optional is  empty
```cpp
struct element_iterator {
    explicit element_iterator(const char* p)
        : cur_(p) {
        current_ = read_next();
    }
    const Element& operator*() const { return *current_; }
    element_iterator& operator++() {
        current_ = read_next();
        return *this;
    }
    friend bool operator==(const element_iterator& it, std::default_sentinel_t) {
        return !it.current_.has_value();
    }
private:
    const char* cur_ = nullptr;
    std::optional<Element> current_;
    std::optional<Element> read_next() { }
};
```
---
## std::ranges::view_interface
Avoiding boilerplate
.empty(), .size(), .front(), .back
```cpp
class element_range : public std::ranges::view_interface<element_range> {
public:
    explicit element_range(const char* text) : text_(text) {}
    element_iterator begin() const { return element_iterator{text_}; }
    std::default_sentinel_t end() const { return {}; }
private:
    const char* text_;
};
```
---
## Compile-time
- Heavy lifting is done by the type system.
- building the pipeline is done at compile time
- runtime; iterating over the items.
- You could get many specializations for a view (moving_average_view)
- code size can explode
- compile times can explode, any_view: P3411R0
---

ex4.cpp
ex5.cpp
ex6.cpp

---
<!-- _class: final-slide -->