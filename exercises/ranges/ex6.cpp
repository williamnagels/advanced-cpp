#include <algorithm>
#include <cassert>
#include <cstdint>
#include <ranges>
#include <vector>

namespace
{
/*
GOAL:
Revisit the interleaved channel image from ex2 using C++23 views instead of a
custom legacy iterator.

Each 12-byte image block contains three consecutive 4-byte channel chunks:

    [ R0 R1 R2 R3 | G0 G1 G2 G3 | B0 B1 B2 B3 ]

1. Split image into 4-byte chunks with std::views::chunk.
2. Build red, green, and blue streams. Start each stream at its channel and
   use std::views::stride to select every third chunk.
3. Zip the three streams so each iteration yields the R, G, and B chunks for
   one image block.
4. Verify the number of RGB blocks and the contents of their channel chunks.
*/
void test_1() {
    std::vector<std::uint8_t> image;
    for (std::uint8_t block = 0; block < 3; ++block) {
        for (int byte = 0; byte < 4; ++byte) image.push_back(10 + block);
        for (int byte = 0; byte < 4; ++byte) image.push_back(20 + block);
        for (int byte = 0; byte < 4; ++byte) image.push_back(30 + block);
    }

    // TODO: Split image into four-byte channel chunks.
    // auto channelChunks = ...;

    // TODO: Create red, green, and blue streams from channelChunks.
    // auto red = ...;
    // auto green = ...;
    // auto blue = ...;

    // TODO: Zip the streams so each row contains one R, G, B chunk.
    // auto rgbBlocks = ...;

    /* Uncomment once the pipeline has been implemented.
    assert(std::ranges::distance(rgbBlocks) == 3);

    auto&& [red0, green0, blue0] = *rgbBlocks.begin();
    assert(std::ranges::equal(red0, std::vector<std::uint8_t>{10, 10, 10, 10}));
    assert(std::ranges::equal(green0, std::vector<std::uint8_t>{20, 20, 20, 20}));
    assert(std::ranges::equal(blue0, std::vector<std::uint8_t>{30, 30, 30, 30}));

    auto&& [red2, green2, blue2] = *std::ranges::next(rgbBlocks.begin(), 2);
    assert(std::ranges::equal(red2, std::vector<std::uint8_t>{12, 12, 12, 12}));
    assert(std::ranges::equal(green2, std::vector<std::uint8_t>{22, 22, 22, 22}));
    assert(std::ranges::equal(blue2, std::vector<std::uint8_t>{32, 32, 32, 32}));
    */
}
}

void ranges_ex6()
{
    test_1();
}