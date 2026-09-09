#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <iostream>
#include <cassert>

namespace
{
struct Player {
    std::string name;
    int score;
};

/*
GOAL:
The goal of this exercise is to combine a Range "Action" (sorting) with a 
Range "View" pipeline (filter -> enumerate -> transform).

1. Sort: Before assigning ranks, the leaderboard must be sorted by score 
   in descending order (highest score first).
2. Filter: Remove any players with a score below 1000.
3. Use an std::view to auto generate the rank (1...N) from positin in the container.
4. Transform: Format the output string using structured bindings [index, player].
*/
void test_1() {
    std::vector<Player> leaderboard = {
        {"NerdSlayer", 2500},
        {"UrMom", 450},
        {"Hotboy69", 1800},
        {"PumperLord", 900},
        {"PossiblyPutin", 3200}
    };

    /*
    auto rankedList =
    */
    /*
    TODO: uncomment these asserts when code above has been fixed
    auto resultCount = std::ranges::distance(rankedList);
    assert(resultCount == 3);
    auto it = rankedList.begin();
    assert(*it == "Rank #1: PossiblyPutin (3200)");
    auto second = std::ranges::next(it, 1);
    assert(*second == "Rank #2: NerdSlayer (2500)");
    auto third = std::ranges::next(it, 2);
    assert(*third == "Rank #3: Hotboy69 (1800)");
    */
}
}

void ranges_ex5()
{
    test_1();
}