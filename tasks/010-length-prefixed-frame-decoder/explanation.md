# Task 010 — Length-Prefixed TCP Frame Decoder: Explanation

## The problem

TCP gives us a stream of bytes. It does **not** preserve application-message boundaries.

A single call that receives TCP data may contain:

- only part of the 4-byte header;
- the full header but only part of the payload;
- exactly one complete frame;
- multiple complete frames;
- the end of one frame plus the beginning of the next.

The protocol for this task defines every frame as:

```text
[4-byte unsigned payload length, big-endian][payload bytes]
```

For example, the payload:

```text
hello
```

has length 5, so the encoded frame is conceptually:

```text
00 00 00 05 h e l l o
```

The decoder therefore needs to keep incomplete data between calls.

---

## Simple design

The implementation uses one member buffer:

```cpp
std::string buffer_;
```

Every call to `append()` simply adds newly received bytes:

```cpp
void append(std::string_view bytes)
{
    buffer_.append(bytes);
}
```

No parsing is done in `append()`.

All parsing is done when `next_frame()` is called.

This keeps the implementation small and easy to reason about.

---

## Step 1: wait for the complete header

The header is always 4 bytes.

```cpp
if (buffer_.size() < 4U) {
    return std::nullopt;
}
```

For example:

```text
00 00 00
```

is not enough to determine the payload length.

The decoder leaves those bytes in `buffer_` and waits for more data.

---

## Step 2: decode the big-endian length

Suppose the first four bytes are:

```text
00 00 00 05
```

They represent the integer 5.

The implementation reads each byte separately:

```cpp
const auto b0 =
    static_cast<std::uint32_t>(static_cast<unsigned char>(buffer_[0]));
const auto b1 =
    static_cast<std::uint32_t>(static_cast<unsigned char>(buffer_[1]));
const auto b2 =
    static_cast<std::uint32_t>(static_cast<unsigned char>(buffer_[2]));
const auto b3 =
    static_cast<std::uint32_t>(static_cast<unsigned char>(buffer_[3]));
```

Then combines them:

```cpp
const std::uint32_t length =
    (b0 << 24U) |
    (b1 << 16U) |
    (b2 << 8U) |
    b3;
```

For:

```text
00 00 00 05
```

this produces:

```text
5
```

This is network byte order: the most significant byte comes first.

### Why convert through `unsigned char`?

A plain C++ `char` may be signed.

For example, a byte containing:

```text
FF
```

could otherwise be interpreted as `-1`.

Converting through `unsigned char` ensures every byte is treated as a value from 0 to 255 before the bit shifts.

---

## Step 3: reject an oversized frame

After decoding the declared payload length:

```cpp
const auto payload_size = static_cast<std::size_t>(length);
```

the decoder checks it against the configured maximum:

```cpp
if (payload_size > max_frame_size_) {
    throw std::length_error("frame exceeds maximum size");
}
```

This prevents the protocol peer from declaring an unexpectedly large frame.

For example, if:

```text
max_frame_size = 1024
```

and the header declares:

```text
5000000
```

the decoder rejects it immediately.

---

## Step 4: wait for the complete payload

Knowing the payload length does not mean the complete payload has already arrived.

Suppose the stream currently contains:

```text
00 00 00 07 n e t
```

The header says the payload has 7 bytes:

```text
network
```

but only 3 payload bytes are currently available.

The code checks:

```cpp
if (buffer_.size() - 4U < payload_size) {
    return std::nullopt;
}
```

Nothing is removed from the buffer.

Later, another TCP read may append:

```text
work
```

and the buffer becomes:

```text
00 00 00 07 n e t w o r k
```

Now the full frame is available.

---

## Step 5: extract the payload

Once the full payload is available:

```cpp
std::string payload = buffer_.substr(4U, payload_size);
```

For:

```text
00 00 00 07 n e t w o r k
```

the returned string is:

```text
network
```

The four header bytes are not part of the returned payload.

---

## Step 6: remove the consumed frame

After extracting the payload:

```cpp
buffer_.erase(0U, 4U + payload_size);
```

This removes exactly:

```text
header + payload
```

and leaves any later frames untouched.

For example:

```text
[00 00 00 03][o n e][00 00 00 03][t w o]
       frame 1             frame 2
```

The first call to:

```cpp
next_frame()
```

returns:

```text
one
```

and leaves:

```text
[00 00 00 03][t w o]
```

The next call returns:

```text
two
```

This naturally supports multiple frames received in a single TCP chunk.

---

## Fragmented TCP example

Assume the peer sends:

```text
[00 00 00 05][hello]
```

TCP may deliver it like this:

```text
append #1: 00
append #2: 00 00
append #3: 05 h
append #4: e l
append #5: l o
```

After each `append()`, the bytes accumulate in `buffer_`.

Until all 4 header bytes exist, `next_frame()` returns:

```cpp
std::nullopt
```

After the header exists, the decoder knows the payload length is 5.

Until all 5 payload bytes exist, it still returns:

```cpp
std::nullopt
```

Once all bytes exist, it returns:

```text
hello
```

---

## Empty frames

A payload length of zero is valid.

The encoded frame is simply:

```text
00 00 00 00
```

Once those four bytes are available, `next_frame()` returns an empty `std::string`.

This is different from returning `std::nullopt`.

- `std::nullopt` means no complete frame is available.
- `std::string{}` means a complete frame with an empty payload was received.

---

## Core algorithm

In simplified pseudocode:

```text
append(bytes):
    append bytes to buffer

next_frame():
    if fewer than 4 bytes:
        return no frame

    decode payload length

    if payload length is too large:
        throw

    if complete payload has not arrived:
        return no frame

    copy payload

    remove header and payload from buffer

    return payload
```

---

## Complexity

Let the payload contain `n` bytes.

Reading the 4-byte header is constant time:

```text
O(1)
```

Copying the payload is:

```text
O(n)
```

The simple implementation also uses:

```cpp
buffer_.erase(0, ...)
```

which may move remaining bytes in the string.

That makes this version less efficient than a production high-throughput decoder that keeps a read offset or uses a ring buffer.

For an interview exercise, however, this version is intentionally simple and correct.

---

## Important interview point

The main concept being tested is usually not bit shifting.

The important networking concept is:

> TCP is a byte stream, not a message protocol.

You cannot assume that one call to `recv()` corresponds to one application frame.

The receiver therefore needs an application-level framing rule.

This task uses one common framing scheme:

```text
[length][payload]
```

Other protocols may use:

- delimiters;
- fixed-size records;
- length-prefix fields of different sizes;
- self-describing message formats.

The decoder must always handle arbitrary fragmentation and coalescing of bytes by TCP.
