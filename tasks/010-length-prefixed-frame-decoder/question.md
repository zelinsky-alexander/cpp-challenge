# Task 010 — Length-Prefixed TCP Frame Decoder

## Goal

Implement a streaming decoder for a simple TCP application protocol.

Each frame is encoded as:

```text
[4-byte unsigned payload length, big-endian][payload bytes]
```

Implement:

```cpp
class FrameDecoder {
public:
    explicit FrameDecoder(std::size_t max_frame_size);

    void append(std::string_view bytes);
    [[nodiscard]] std::optional<std::string> next_frame();
};
```

`append()` receives arbitrary chunks from a TCP byte stream. A chunk may contain part of a header, part of a payload, exactly one frame, or multiple frames.

`next_frame()` returns the next complete payload when available. If no complete frame is available, it returns `std::nullopt`.

## Requirements

- Use C++20.
- Interpret the 4-byte length field as an unsigned 32-bit integer in network byte order (big-endian).
- Preserve bytes that belong to incomplete frames across calls to `append()`.
- Support multiple complete frames arriving in one `append()` call.
- Return frames in stream order.
- An empty payload (`length == 0`) is a valid frame.
- Reject a frame whose declared payload length exceeds `max_frame_size` by throwing `std::length_error`.
- Do not use global state.
- Avoid undefined behavior and out-of-bounds access when input is fragmented at any byte boundary.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/010-length-prefixed-frame-decoder/solution.cpp \
  -o /tmp/cpp-challenge-010

/tmp/cpp-challenge-010
```

Sanitizer build:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion \
  -g -fsanitize=address,undefined -fno-omit-frame-pointer \
  tasks/010-length-prefixed-frame-decoder/solution.cpp \
  -o /tmp/cpp-challenge-010-san

/tmp/cpp-challenge-010-san
```
