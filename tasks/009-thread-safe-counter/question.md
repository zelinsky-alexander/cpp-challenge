# Task 009 — Thread-Safe Counter

## Goal

Implement:

```cpp
class ThreadSafeCounter {
public:
    void increment();
    void add(std::int64_t amount);
    [[nodiscard]] std::int64_t value() const;
};
```

The counter may be accessed concurrently by multiple threads.

## Requirements

- Use C++20.
- `increment()`, `add()`, and `value()` must be safe when called concurrently.
- The final value must include every completed increment and addition exactly once.
- Do not use global state.
- Do not hold a lock longer than necessary.
- The class must remain valid when many threads repeatedly update it.

## Edge cases

Consider:

- No updates.
- One thread.
- Many threads.
- Positive and negative additions.
- Concurrent reads while updates are happening.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -pthread -g \
  tasks/009-thread-safe-counter/solution.cpp \
  -o /tmp/cpp-challenge-009

/tmp/cpp-challenge-009
```

Sanitizer build:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -pthread \
  -g -fsanitize=address,undefined -fno-omit-frame-pointer \
  tasks/009-thread-safe-counter/solution.cpp \
  -o /tmp/cpp-challenge-009-san

/tmp/cpp-challenge-009-san
```
