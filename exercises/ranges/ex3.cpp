#include <vector>
#include <cstdint>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <concepts>
#include <ranges>

namespace
{
    struct TlvPacket {
        uint8_t type;
        uint8_t length;
        const uint8_t* payload;
    };

    /*
    GOAL:
    C++20 fundamentally changed how iterators interface with the standard library. 
    Instead of inheriting from `std::iterator` (which is deprecated), C++20
    algorithms express iterator requirements with concepts.

    By defining the modern type aliases (`iterator_concept`, `value_type`, etc.) 
    and providing the correct operator semantics, your class automatically satisfies 
    the `std::forward_iterator` concept.

    PACKET LAYOUT:
    You are parsing a raw byte buffer of TLV (Type-Length-Value) network packets.
    Each packet has a variable size:
    [ Type (1 byte) | Length (1 byte) | Payload ('Length' bytes) ]

    TASK:
    1. Define the necessary C++20 iterator aliases.
    2. Complete the dereference and increment operators to jump packet-to-packet.
    3. Rely on C++20 operator synthesis (see comment below) for the equality check.
    */

    class TlvIterator {
    public:
        // TODO: Define the essential type aliases for C++20 iterator concepts.
        // To satisfy std::forward_iterator, provide value_type and difference_type.
        // To explicitly opt into forward (multi-pass) semantics rather than falling 
        // back to an input_iterator, define `iterator_concept` as well.
        
        // using iterator_concept = ...
        // using value_type = ...
        // using difference_type = ...
        
        TlvIterator() = default;
        explicit TlvIterator(const uint8_t* ptr) : m_ptr(ptr) {}

        // TODO: Implement the dereference operator.
        // It should construct and return a `TlvPacket` by reading the current memory location.
        // TlvPacket operator*() const { ... }

        // TODO: Implement the prefix increment operator (++it).
        // Advance `m_ptr` to the start of the next packet. 
        // Hint: The current packet occupies (2 + current length) bytes.
        // TlvIterator& operator++() { ... }

        // TODO: Implement the postfix increment operator (it++).
        // TlvIterator operator++(int) { ... }

        // TODO: Implement the equality operator. 
        // Note: In C++20, you only need to define `operator==`. The compiler 
        // automatically synthesizes `operator!=` for you.
        // friend bool operator==(const TlvIterator& a, const TlvIterator& b) { ... }

    private:
        const uint8_t* m_ptr = nullptr;
    };

    // TODO: Uncomment this static_assert once your iterator is fully implemented 
    // to verify that it satisfies the C++20 std::forward_iterator concept constraints.
    // static_assert(std::forward_iterator<TlvIterator>, "TlvIterator does not satisfy std::forward_iterator!");

    // Helper to get the end iterator
    TlvIterator make_tlv_end(const std::vector<uint8_t>& buffer) {
        return TlvIterator(buffer.data() + buffer.size());
    }

    void test_3() 
    {
        // Dummy TLV buffer simulating memory-mapped packet data
        // Packet 1: Type 0x01, Len 0x02, Payload [0xAA, 0xBB]
        // Packet 2: Type 0x02, Len 0x00, Payload []
        // Packet 3: Type 0x03, Len 0x04, Payload [0x11, 0x22, 0x33, 0x44]
        std::vector<uint8_t> buffer = {
            0x01, 0x02, 0xAA, 0xBB,
            0x02, 0x00,
            0x03, 0x04, 0x11, 0x22, 0x33, 0x44
        };

        TlvIterator begin(buffer.data());
        TlvIterator end = make_tlv_end(buffer);

        int packetCount = 0;
        int totalPayloadBytes = 0;
        int typeChecksum = 0;

        // TODO: Construct std::ranges::subrange packets{begin, end}, then use a
        // range-based for loop. Count packets, payload bytes, and packet types.

        // Uncomment once the exercise has been implemented.
        // assert(packetCount == 3);
        // assert(totalPayloadBytes == 6);
        // assert(typeChecksum == 6);
    }
}

void ranges_ex3()
{
    test_3();
}