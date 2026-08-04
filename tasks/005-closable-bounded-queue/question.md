# Task 005 — Closable Bounded Blocking Queue

## Goal

Implement a generic, thread-safe bounded blocking queue:

```cpp
template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity);

    bool push(T value);
    std::optional<T> pop();
    void close();

    [[nodiscard]] bool isClosed() const;
    [[nodiscard]] std::size_t size() const;
};
```

The queue is intended for a producer/consumer pipeline.

- `push(value)` waits while the queue is full.
- `push(value)` inserts the value and returns `true` when the queue is open.
- `push(value)` returns `false` without inserting when the queue has been closed.
- `pop()` waits while the queue is empty and still open.
- After `close()`, consumers may continue removing already queued values.
- `pop()` returns `std::nullopt` only when the queue is both closed and empty.
- `close()` is idempotent and must wake every blocked producer and consumer.

## Requirements

- Use C++20.
- Target Linux with `std::thread`, `std::mutex`, and `std::condition_variable`.
- Use a standard-library container for FIFO storage.
- Do not busy-wait, sleep, or poll.
- Do not hold the mutex while copying or moving a removed value longer than necessary.
- Correctly handle spurious wakeups by using predicates.
- The queue capacity must be greater than zero; reject zero capacity with `std::invalid_argument`.
- Support move-only values such as `std::unique_ptr<int>`.
- `size()` and `isClosed()` must be data-race-free.

## Concurrency contract

Assume:

- Multiple producers may call `push()` concurrently.
- Multiple consumers may call `pop()` concurrently.
- Any thread may call `close()`.
- The object itself remains alive until all operations have returned.

You do not need to implement timed waits, cancellation tokens, lock-free behavior, or destruction while other threads are still using the queue.

## Invariants to explain in `solution.cpp`

Add brief comments describing:

1. Why the number of queued elements never exceeds `capacity`.
2. Which shared fields are protected by the mutex.
3. The predicate used by blocked producers.
4. The predicate used by blocked consumers.
5. Why `close()` must notify both condition variables.
6. Why queued values are drained after closure instead of discarded.

## Edge cases

Cover at least:

- Capacity `1`.
- FIFO ordering.
- Zero-capacity construction.
- A producer blocked by a full queue.
- A consumer blocked by an empty queue.
- Closing an empty queue.
- Closing a non-empty queue.
- Calling `close()` more than once.
- Move-only values.
- Multiple producers and consumers completing without lost or duplicated values.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -pthread -g \
  tasks/005-closable-bounded-queue/solution.cpp \
  -o /tmp/cpp-challenge-005

/tmp/cpp-challenge-005
```

The initial scaffold compiles, but assertions intentionally fail until the TODOs are implemented.

Sanitizer build:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -pthread \
  -g -fsanitize=address,undefined -fno-omit-frame-pointer \
  tasks/005-closable-bounded-queue/solution.cpp \
  -o /tmp/cpp-challenge-005-san

/tmp/cpp-challenge-005-san
```

When available, also run a ThreadSanitizer build separately:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -pthread \
  -g -fsanitize=thread \
  tasks/005-closable-bounded-queue/solution.cpp \
  -o /tmp/cpp-challenge-005-tsan

/tmp/cpp-challenge-005-tsan
```
