#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <cassert>

namespace
{
struct Player {
    std::string name;
    int score;
};

/*
GOAL:
Combine a range action (sorting) with a view pipeline:
filter -> enumerate -> transform.

1. Sort the leaderboard by score, highest first.
2. Filter out players whose score is below 1000.
3. Use std::views::enumerate to generate a zero-based index.
4. Transform each [index, player] pair into the requested rank string.

Use the &Player::score projection when sorting instead of writing a comparator
that compares complete Player objects.
*/
void test_1() {
    std::vector<Player> leaderboard = {
        {"Noor", 2500},
        {"Mina", 450},
        {"Luis", 1800},
        {"Priya", 900},
        {"Akira", 3200}
    };

    // TODO: Sort leaderboard descending with std::ranges::sort, a standard
    // comparator, and the &Player::score projection.

    // TODO: Build rankedList as filter -> enumerate -> transform.
    // Accept the tuple-like enumerate result as auto&& before decomposing it.
    // auto rankedList = ...;

    /*
    TODO: Uncomment once rankedList has been implemented.
    auto resultCount = std::ranges::distance(rankedList);
    assert(resultCount == 3);
    auto it = rankedList.begin();
    assert(*it == "Rank #1: Akira (3200)");
    auto second = std::ranges::next(it, 1);
    assert(*second == "Rank #2: Noor (2500)");
    auto third = std::ranges::next(it, 2);
    assert(*third == "Rank #3: Luis (1800)");
    */
}
}

void ranges_ex8()
{
    test_1();
}
