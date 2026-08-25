# Task 023 — Sliding Window

## Goal

Implement:

```cpp
std::optional<long long> maxWindowSum(
    const std::vector<int>& values,
    std::size_t windowSize);
```

Return the largest sum of any contiguous window containing exactly `windowSize` elements.

Return `std::nullopt` when `windowSize == 0` or when the requested window is larger than the input.

## Examples

```text
values = {2, 1, 5, 1, 3, 2}, windowSize = 3 -> 9
values = {-4, -2, -7}, windowSize = 2 -> -6
values = {1, 2}, windowSize = 3 -> nullopt
```

## Requirements

- Use C++20.
- Do not modify the input.
- Do not recompute each window sum from scratch.
- Target time complexity: `O(n)`.
- Target extra-space complexity: `O(1)`.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/023-sliding-window/solution.cpp -o /tmp/cpp-challenge-023
/tmp/cpp-challenge-023
```
