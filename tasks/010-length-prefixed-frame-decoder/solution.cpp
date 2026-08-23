#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

class FrameDecoder {
public:
    explicit FrameDecoder(std::size_t max_frame_size)
        : max_frame_size_(max_frame_size)
    {
    }

    void append(std::string_view bytes)
    {
        // TODO: implement.
        (void)bytes;
    }

    [[nodiscard]] std::optional<std::string> next_frame()
    {
        // TODO: implement.
        return std::nullopt;
    }

private:
    std::size_t max_frame_size_;
    // TODO: add decoder state.
};

namespace {

std::string make_frame(std::string_view payload)
{
    assert(payload.size() <= static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max()));

    const auto length = static_cast<std::uint32_t>(payload.size());
    std::string frame;
    frame.reserve(4U + payload.size());

    frame.push_back(static_cast<char>((length >> 24U) & 0xFFU));
    frame.push_back(static_cast<char>((length >> 16U) & 0xFFU));
    frame.push_back(static_cast<char>((length >> 8U) & 0xFFU));
    frame.push_back(static_cast<char>(length & 0xFFU));
    frame.append(payload);

    return frame;
}

void test_single_complete_frame()
{
    FrameDecoder decoder{1024};
    decoder.append(make_frame("hello"));

    const auto frame = decoder.next_frame();
    assert(frame.has_value());
    assert(*frame == "hello");
    assert(!decoder.next_frame().has_value());
}

void test_header_fragmented_byte_by_byte()
{
    FrameDecoder decoder{1024};
    const std::string encoded = make_frame("abc");

    for (std::size_t i = 0; i < 4U; ++i) {
        decoder.append(std::string_view{encoded}.substr(i, 1U));
        assert(!decoder.next_frame().has_value());
    }

    decoder.append(std::string_view{encoded}.substr(4U));
    const auto frame = decoder.next_frame();
    assert(frame.has_value());
    assert(*frame == "abc");
}

void test_payload_fragmented()
{
    FrameDecoder decoder{1024};
    const std::string encoded = make_frame("network");

    decoder.append(std::string_view{encoded}.substr(0U, 6U));
    assert(!decoder.next_frame().has_value());

    decoder.append(std::string_view{encoded}.substr(6U, 2U));
    assert(!decoder.next_frame().has_value());

    decoder.append(std::string_view{encoded}.substr(8U));
    const auto frame = decoder.next_frame();
    assert(frame.has_value());
    assert(*frame == "network");
}

void test_multiple_frames_in_one_chunk()
{
    FrameDecoder decoder{1024};
    const std::string input = make_frame("one") + make_frame("two") + make_frame("three");

    decoder.append(input);

    assert(decoder.next_frame() == std::optional<std::string>{"one"});
    assert(decoder.next_frame() == std::optional<std::string>{"two"});
    assert(decoder.next_frame() == std::optional<std::string>{"three"});
    assert(!decoder.next_frame().has_value());
}

void test_empty_payload()
{
    FrameDecoder decoder{1024};
    decoder.append(make_frame(""));

    const auto frame = decoder.next_frame();
    assert(frame.has_value());
    assert(frame->empty());
}

void test_frame_split_followed_by_second_frame()
{
    FrameDecoder decoder{1024};
    const std::string first = make_frame("first");
    const std::string second = make_frame("second");

    decoder.append(std::string_view{first}.substr(0U, 5U));
    assert(!decoder.next_frame().has_value());

    decoder.append(std::string_view{first}.substr(5U) + second);

    assert(decoder.next_frame() == std::optional<std::string>{"first"});
    assert(decoder.next_frame() == std::optional<std::string>{"second"});
}

void test_oversized_frame_is_rejected()
{
    FrameDecoder decoder{3};

    bool threw = false;
    try {
        decoder.append(make_frame("four"));
        (void)decoder.next_frame();
    } catch (const std::length_error&) {
        threw = true;
    }

    assert(threw);
}

} // namespace

int main()
{
    test_single_complete_frame();
    test_header_fragmented_byte_by_byte();
    test_payload_fragmented();
    test_multiple_frames_in_one_chunk();
    test_empty_payload();
    test_frame_split_followed_by_second_frame();
    test_oversized_frame_is_rejected();
}
