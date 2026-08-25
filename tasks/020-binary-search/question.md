# Task 020 — Binary Search

## Goal

Implement:

```cpp
std::optional<std::size_t> binarySearch(
    const std::vector<int>& values,
    int target);
```

`values` is sorted in ascending order. Return the index of `target` when found, otherwise return `std::nullopt`.

If the target appears more than once, returning any matching index is acceptable.

## Examples

```text
values = {1, 3, 5, 7, 9}, target = 7 -> 3
values = {1, 3, 5, 7, 9}, target = 4 -> nullopt
values = {}, target = 1 -> nullopt
```

## Requirements

- Use C++20.
- Do not modify the input.
- Use an iterative implementation.
- Target time complexity: `O(log n)`.
- Target extra-space complexity: `O(1)`.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/020-binary-search/solution.cpp -o /tmp/cpp-challenge-020
/tmp/cpp-challenge-020
```
