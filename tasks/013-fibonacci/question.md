# Task 013 — Fibonacci Number

## Goal

Implement:

```cpp
std::uint64_t fibonacci(std::size_t n);
```

Return the `n`th Fibonacci number.

The Fibonacci sequence is defined as:

```text
F(0) = 0
F(1) = 1
F(n) = F(n - 1) + F(n - 2)
```

## Examples

```text
fibonacci(0)  -> 0
fibonacci(1)  -> 1
fibonacci(2)  -> 1
fibonacci(5)  -> 5
fibonacci(10) -> 55
```

## Requirements

- Use C++20.
- Do not use recursion.
- Target time complexity: `O(n)`.
- Target extra-space complexity: `O(1)`.
- Return the correct result for every `n` from `0` through `93` inclusive.

## Edge cases

Cover at least:

- `n == 0`.
- `n == 1`.
- Small sequence values.
- A larger valid value such as `n == 50`.
- The largest Fibonacci number that fits in `std::uint64_t`, `n == 93`.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/013-fibonacci/solution.cpp \
  -o /tmp/cpp-challenge-013

/tmp/cpp-challenge-013
```
