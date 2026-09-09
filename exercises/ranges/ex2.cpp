#include <vector>
#include <cstdint>
#include <numeric>
#include <cassert>
#include <iterator>

namespace
{
    enum class Channel {
        R = 0,
        G = 1,
        B = 2
    };

    /*
    GOAL:
    The goal of this exercise is to write a custom legacy iterator that navigates a specific
    memory layout, allowing standard algorithms to seamlessly process complex data.

    IMAGE LAYOUT:
    You have a 3-channel RGB image stored as a flat byte blob (std::vector<uint8_t>).
    The pixels are interleaved in chunks of 4 bytes per channel.
    A single 12-byte block looks like this:
    [ R0, R1, R2, R3,  G0, G1, G2, G3,  B0, B1, B2, B3 ]
       0   1   2   3    4   5   6   7    8   9  10  11

    If you are iterating over the Blue (B) channel, your iterator must yield indices:
    8, 9, 10, 11 ... then skip 8 bytes (the R and G of the next block) ... 20, 21, 22, 23.

    TASK:
    1. Complete the 'operator++()' in the ChannelIterator.
    2. Use 'std::accumulate' to sum the Green (G) channel in the test function.
       Make sure your iterator satisfies the named requirements for std::accumulate to work.
    */

    class ChannelIterator {
    public:
        // TODO: Define the five legacy iterator traits.
        // These traits are used by STL algorithms to determine how to interact with your iterator.
        // Model LegacyInputIterator; random access is unnecessary here.
        
        // Constructor
        ChannelIterator(const uint8_t* ptr, const uint8_t* end)
            : m_ptr(ptr), m_end(end), m_pos_in_chunk(0) {}

        //TODO: Implement the dereference operator
        /*reference operator*() const {
            return *m_ptr;
        }*/

        // TODO: Implement the prefix increment operator
        ChannelIterator& operator++() {
            // 1. Advance the pointer by 1 byte.
            // 2. Increment your position within the current 4-byte chunk.
            // 3. After 4 bytes, skip the other two channels in the next block.
            // 4. Clamp m_ptr to m_end if the skip moved beyond the buffer. This
            //    lets equality remain a real equivalence relation.
            
            // Your code here...

            return *this;
        }

        // Postfix increment
        ChannelIterator operator++(int) {
            ChannelIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator!=(const ChannelIterator& a, const ChannelIterator& b) {
            return !(a == b);
        }

        friend bool operator==(const ChannelIterator& a, const ChannelIterator& b) {
            return a.m_ptr == b.m_ptr;
        }

    private:
        const uint8_t* m_ptr;
        const uint8_t* m_end;
        int m_pos_in_chunk;
    };

    // Helper functions to generate the begin/end iterators for a specific channel
    ChannelIterator make_channel_begin(const std::vector<uint8_t>& image, Channel c) {
        // Offset by 0 for R, 4 for G, 8 for B
        int offset = static_cast<int>(c) * 4;
        return ChannelIterator(image.data() + offset,
                       image.data() + image.size());
    }

    ChannelIterator make_channel_end(const std::vector<uint8_t>& image) {
        return ChannelIterator(image.data() + image.size(),
                       image.data() + image.size());
    }

    void test_2() 
    {
        // Generate a dummy image of 10 blocks (12 bytes per block = 120 bytes)
        std::vector<uint8_t> image(120);
        for (size_t i = 0; i < 10; ++i) {
            for(int j = 0; j < 4; ++j) image[i * 12 + 0 + j] = 1; // R channel = 1
            for(int j = 0; j < 4; ++j) image[i * 12 + 4 + j] = 2; // G channel = 2
            for(int j = 0; j < 4; ++j) image[i * 12 + 8 + j] = 3; // B channel = 3
        }

        // 1. Manual "Do-It-All" loop to sum the G channel
        int rawGSum = 0;
        for (size_t i = 0; i < image.size(); i += 12) {
            for (int j = 0; j < 4; ++j) {
                rawGSum += image[i + 4 + j];
            }
        }
        assert(rawGSum == 80); // 10 blocks * 4 bytes * value(2) = 80

        // 2. The STL Way
        int stlGSum = 0;
        
        // TODO: Use std::accumulate with make_channel_begin() and make_channel_end() 
        // to calculate stlGSum for the Green channel.
        // What named requirements must be satisfied for this to work? (Hint: check the std::accumulate documentation)
        
        // Uncomment once the exercise has been implemented.
        // assert(stlGSum == 80);
    }
}

void ranges_ex2()
{
    test_2();
}