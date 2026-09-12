#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <functional>

namespace
{
struct Player {
    std::string name;
    int score;
};

void test_1()
{
    std::vector<Player> leaderboard = {
        {"Noor", 2500},
        {"Mina", 450},
        {"Luis", 1800},
        {"Priya", 900},
        {"Akira", 3200}
    };

    // The projection makes greater compare scores instead of Player objects.
    std::ranges::sort(leaderboard, std::greater{}, &Player::score);

    auto rankedList = leaderboard
        | std::views::filter([](const Player& player) {
              return player.score >= 1000;
          })
        | std::views::enumerate
        | std::views::transform([](auto&& indexedPlayer) {
              auto&& [index, player] = indexedPlayer;
              return "Rank #" + std::to_string(index + 1) + ": " +
                     player.name + " (" + std::to_string(player.score) + ")";
          });

    auto resultCount = std::ranges::distance(rankedList);
    assert(resultCount == 3);
    auto it = rankedList.begin();
    assert(*it == "Rank #1: Akira (3200)");
    auto second = std::ranges::next(it, 1);
    assert(*second == "Rank #2: Noor (2500)");
    auto third = std::ranges::next(it, 2);
    assert(*third == "Rank #3: Luis (1800)");
}
}

void ranges_ex8()
{
    test_1();
}