# Advanced C++ Training

The training is organized into five main topics:
 - Ranges
 - Coroutines
 - Modules
 - Various
 - Tooling
 
The training assumes you have prior experience with containers, move semantics and basic metaprogramming.
A list of required knowledge is provided at the bottom of this document.
If any of those topics listed at the bottom are unfamiliar to you, I recommend reviewing them in advance to get the most out of the course.

---

# Training Topics

## 1. Ranges
**Why?**
Ranges offer composability, lazy evaluation, and cleaner syntax. Compared to raw for loops and STL algorithms, ranges lead to more declarative, readable code.

**What?**
- Intro to views and actions
- Lazy transformations, filters, joins
- Composing pipelines
- Customizing views

## 2. Coroutines

**Why?**
Coroutines allow you to write sequential-looking code that can be executed asynchronously behind the scenes.
This leads to more natural control flow and simplifies asynchronous programming.
By using coroutines, you can avoid nested callbacks, also known as the dreaded “callback hell”.

**What?**
- co_yield, co_return, co_await
- Generator-based iteration
- Coroutine lifetimes and pitfalls
- Integration with ranges and event loops

## 3. Modules

**Why?**
Modules introduce a modern way to organize code in large C++ projects.
You will find out how modules, ranges, and concepts are designed to work together and see the bigger modern c++ picture.

**What?**
- Importable module units
- Internal vs. exported symbols
- Module partitions and interface units
- Working with legacy headers

## 4. Various

**Why?**
“quality-of-life” features every C++ developer should be using.

**What?**
- Spaceship operator
- Structured bindings
- std::format
- std::span
- std::expected<T, E>

## 5. Tooling & Static Analysis

**Why?**
Understanding your tooling improves code quality, developer productivity, and catches bugs before runtime.

**What?**
- Compile-time analysis (clang-tidy, cppcheck)
- Sanitizers (ASan, UBSan, TSan)
- Compiler warnings, flags, and profiles
- CMake integration

# Knowledge requirements

You **must be comfortable with** the following modern C++ concepts and language features. If any of the examples below are unclear to you, please read up on the topics beforehand

---
## 1. Concepts:

You should be familiar with **C++20 concepts**. We will be using concepts to constrain template parameters.
Knowledge of SFINEA is useful but not required.

```cpp
template<typename T>
concept Loggable = requires(T t) {
    { t.timestamp } -> std::convertible_to<std::chrono::system_clock::time_point>;
};

template<Loggable T>
void process(T&& t) {
    std::cout << "Log: " << t.timestamp.time_since_epoch().count() << '\n';
    forward(t);  // Calls function defined later
}

void forward(auto&& t) {
    std::cout << "Forwarding\n";
}
```

## 2. Move Semantics

You should understand the concept of ownership, move construction, move assignment and ownership transfer.

```cpp 
std::vector<std::string> create() {
    std::vector<std::string> v = {"hello", "world"};
    return v;
}

int main() {
    std::vector<std::string> v = create(); // Moved, not copied
}
```

## 3. Universal References and Perfect Forwarding

You should know how to use T&& in template functions to forward arguments efficiently.

```cpp
template<typename T>
void callAndLog(T&& arg) {
    std::cout << "Calling...\n";
    process(std::forward<T>(arg)); // Perfect forwarding
}
```

## 4. Value Semantics

Understand usecases for std::optional, std::variant and std::any.
Why is std::variant a form of external polymorphism?

```cpp
std::vector<std::optional<int>> data = {
    std::nullopt,
    42,
    7
};

for (auto& opt : data) {
    if (opt) std::cout << "Value: " << *opt << '\n';
    else std::cout << "No value\n";
}
```
