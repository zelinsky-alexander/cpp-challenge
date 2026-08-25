#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

class FrameDecoder {
public:
    explicit FrameDecoder(std::uint32_t maxFrameSize);
    std::vector<std::string> feed(std::span<const std::byte> bytes);
};

int main()
{
    FrameDecoder decoder(1024);

    const std::vector<std::byte> first{
        std::byte{0}, std::byte{0}
    };
    assert(decoder.feed(first).empty());

    const std::vector<std::byte> second{
        std::byte{0}, std::byte{5},
        std::byte{'h'}, std::byte{'e'}
    };
    assert(decoder.feed(second).empty());

    const std::vector<std::byte> third{
        std::byte{'l'}, std::byte{'l'}, std::byte{'o'},
        std::byte{0}, std::byte{0}, std::byte{0}, std::byte{3},
        std::byte{'b'}, std::byte{'y'}, std::byte{'e'},
        std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}
    };
    assert(decoder.feed(third) == std::vector<std::string>({"hello", "bye", ""}));

    FrameDecoder limited(2);
    const std::vector<std::byte> oversized{
        std::byte{0}, std::byte{0}, std::byte{0}, std::byte{3}
    };
    bool threw = false;
    try { static_cast<void>(limited.feed(oversized)); } catch (const std::length_error&) { threw = true; }
    assert(threw);

    return 0;
}
