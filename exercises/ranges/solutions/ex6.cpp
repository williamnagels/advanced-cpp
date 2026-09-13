#include <algorithm>
#include <cassert>
#include <cstdint>
#include <ranges>
#include <vector>

namespace
{
void test_1()
{
    std::vector<std::uint8_t> image;
    for (std::uint8_t block = 0; block < 3; ++block) {
        for (int byte = 0; byte < 4; ++byte) image.push_back(10 + block);
        for (int byte = 0; byte < 4; ++byte) image.push_back(20 + block);
        for (int byte = 0; byte < 4; ++byte) image.push_back(30 + block);
    }

        /*

        Pseudocode using block indices:
        
        block_count = image.size() / bytes_per_rgb_block
        rgb_blocks = indices(0, block_count)
            | transform(index -> {
                offset = index * bytes_per_rgb_block
                return {
                    image[offset + 0 .. offset + 4],
                    image[offset + 4 .. offset + 8],
                    image[offset + 8 .. offset + 12]
                }
            })
        */
    // Every element of channelChunks is one four-byte channel view.
    auto channelChunks = image | std::views::chunk(4);

    // There are three channel chunks per image block.
    auto red = channelChunks | std::views::stride(3);
    // Drop 1 element (R channel) then take every third element for the G channel.
    auto green = channelChunks | std::views::drop(1) | std::views::stride(3);
    auto blue = channelChunks | std::views::drop(2) | std::views::stride(3);

    // zip aligns corresponding channel chunks without copying their bytes.
    auto rgbBlocks = std::views::zip(red, green, blue);

    assert(std::ranges::distance(rgbBlocks) == 3);

    auto&& [red0, green0, blue0] = *rgbBlocks.begin();
    assert(std::ranges::equal(red0, std::vector<std::uint8_t>{10, 10, 10, 10}));
    assert(std::ranges::equal(green0, std::vector<std::uint8_t>{20, 20, 20, 20}));
    assert(std::ranges::equal(blue0, std::vector<std::uint8_t>{30, 30, 30, 30}));

    auto&& [red2, green2, blue2] = *std::ranges::next(rgbBlocks.begin(), 2);
    assert(std::ranges::equal(red2, std::vector<std::uint8_t>{12, 12, 12, 12}));
    assert(std::ranges::equal(green2, std::vector<std::uint8_t>{22, 22, 22, 22}));
    assert(std::ranges::equal(blue2, std::vector<std::uint8_t>{32, 32, 32, 32}));
}
}

void ranges_ex6()
{
    test_1();
}