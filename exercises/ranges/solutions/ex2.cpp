#include <vector>
#include <cstdint>
#include <numeric>
#include <cassert>
#include <iterator>
#include <algorithm>

namespace
{
enum class Channel { R = 0, G = 1, B = 2 };

class ChannelIterator {
public:
    // Legacy algorithms discover an iterator's capabilities through these traits.
    using iterator_category = std::input_iterator_tag;
    using value_type = std::uint8_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const std::uint8_t*;
    using reference = const std::uint8_t&;

    ChannelIterator(const std::uint8_t* ptr, const std::uint8_t* end)
        : m_ptr(ptr), m_end(end), m_pos_in_chunk(0) {}

    reference operator*() const {
        return *m_ptr;
    }

    ChannelIterator& operator++() {
        if (m_ptr == m_end) {
            return *this;
        }

        ++m_ptr;
        ++m_pos_in_chunk;
        if (m_pos_in_chunk == 4) {
            m_pos_in_chunk = 0;
            // Skip the next two channel chunks without passing the end pointer.
            m_ptr += std::min<std::ptrdiff_t>(8, m_end - m_ptr);
        }
        return *this;
    }

    ChannelIterator operator++(int) {
        ChannelIterator previous = *this;
        ++(*this);
        return previous;
    }

    friend bool operator!=(const ChannelIterator& lhs,
                           const ChannelIterator& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator==(const ChannelIterator& lhs,
                           const ChannelIterator& rhs) {
        return lhs.m_ptr == rhs.m_ptr;
    }

private:
    const std::uint8_t* m_ptr;
    const std::uint8_t* m_end;
    int m_pos_in_chunk;
};

ChannelIterator make_channel_begin(const std::vector<std::uint8_t>& image,
                                   Channel channel) {
    int offset = static_cast<int>(channel) * 4;
    return ChannelIterator(image.data() + offset, image.data() + image.size());
}

ChannelIterator make_channel_end(const std::vector<std::uint8_t>& image) {
    return ChannelIterator(image.data() + image.size(),
                           image.data() + image.size());
}

void test_2()
{
    std::vector<std::uint8_t> image(120);
    for (std::size_t i = 0; i < 10; ++i) {
        for (int j = 0; j < 4; ++j) image[i * 12 + j] = 1;
        for (int j = 0; j < 4; ++j) image[i * 12 + 4 + j] = 2;
        for (int j = 0; j < 4; ++j) image[i * 12 + 8 + j] = 3;
    }

    int rawGSum = 0;
    for (std::size_t i = 0; i < image.size(); i += 12) {
        for (int j = 0; j < 4; ++j) rawGSum += image[i + 4 + j];
    }
    assert(rawGSum == 80);

    // accumulate requires a LegacyInputIterator, which ChannelIterator models.
    int stlGSum = std::accumulate(make_channel_begin(image, Channel::G),
                                  make_channel_end(image), 0);
    assert(stlGSum == 80);
}
}

void ranges_ex2()
{
    test_2();
}