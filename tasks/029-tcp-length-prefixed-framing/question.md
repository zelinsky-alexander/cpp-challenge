# Task 029 — TCP Length-Prefixed Framing

## Goal

TCP is a byte stream, so one read may contain part of a message, exactly one message, or several messages.

Implement a decoder for frames encoded as:

```text
[4-byte unsigned big-endian payload length][payload bytes]
```

Use this interface:

```cpp
class FrameDecoder {
public:
    explicit FrameDecoder(std::uint32_t maxFrameSize);

    std::vector<std::string> feed(std::span<const std::byte> bytes);
};
```

Each call to `feed` receives the next bytes from the TCP stream and returns every complete payload decoded during that call. Incomplete bytes must be retained for the next call.

## Requirements

- Use C++20.
- Correctly handle a header split across reads.
- Correctly handle a payload split across reads.
- Correctly handle multiple frames in one read.
- Support zero-length payloads.
- Interpret the 4-byte length in network byte order (big-endian).
- Throw `std::length_error` when a declared payload length exceeds `maxFrameSize`.
- Do not discard bytes belonging to an incomplete frame.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/029-tcp-length-prefixed-framing/solution.cpp -o /tmp/cpp-challenge-029
/tmp/cpp-challenge-029
```
