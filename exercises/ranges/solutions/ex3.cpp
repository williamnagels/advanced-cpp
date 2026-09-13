#include <vector>
#include <cstdint>
#include <iterator>
#include <cassert>
#include <concepts>
#include <ranges>

namespace
{
struct TlvPacket {
    std::uint8_t type;
    std::uint8_t length;
    const std::uint8_t* payload;
};

class TlvIterator {
public:
    /*

    - Declaring forward_iterator promises multi-pass behavior: copying an
        iterator and advancing one copy does not invalidate the other.
    */
    // These aliases opt into multi-pass forward-iterator semantics.
    using iterator_concept = std::forward_iterator_tag;
    using value_type = TlvPacket;
    using difference_type = std::ptrdiff_t;

    TlvIterator() = default;
    explicit TlvIterator(const std::uint8_t* ptr) : m_ptr(ptr) {}

    /*
    - Dereference returns a packet descriptor by value. Its payload pointer is
    still non-owning and remains valid only while the buffer is alive.
    */
    TlvPacket operator*() const {
        return {m_ptr[0], m_ptr[1], m_ptr + 2};
    }

    TlvIterator& operator++() {
        // A packet occupies its two-byte header plus its declared payload.
        m_ptr += 2 + m_ptr[1];
        return *this;
    }

    TlvIterator operator++(int) {
        TlvIterator previous = *this;
        ++(*this);
        return previous;
    }

    friend bool operator==(const TlvIterator& lhs, const TlvIterator& rhs) {
        // C++20 rewrites != in terms of this equality operator.
        return lhs.m_ptr == rhs.m_ptr;
    }

private:
    const std::uint8_t* m_ptr = nullptr;
};

static_assert(std::forward_iterator<TlvIterator>);

TlvIterator make_tlv_end(const std::vector<std::uint8_t>& buffer) {
    return TlvIterator(buffer.data() + buffer.size());
}

void test_3()
{
    std::vector<std::uint8_t> buffer = {
        0x01, 0x02, 0xAA, 0xBB,
        0x02, 0x00,
        0x03, 0x04, 0x11, 0x22, 0x33, 0x44
    };

    TlvIterator begin(buffer.data());
    TlvIterator end = make_tlv_end(buffer);
    int packetCount = 0;
    int totalPayloadBytes = 0;
    int typeChecksum = 0;

    /*
        - What should production parsing do with a truncated header or a length
        beyond the buffer? This iterator trusts validated input;
    */
    std::ranges::subrange packets{begin, end};
    for (TlvPacket packet : packets) {
        ++packetCount;
        totalPayloadBytes += packet.length;
        typeChecksum += packet.type;
    }

    assert(packetCount == 3);
    assert(totalPayloadBytes == 6);
    assert(typeChecksum == 6);
}
}

void ranges_ex3()
{
    test_3();
}