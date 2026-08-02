# Task 003 — Longest Stable Window

## Goal

Implement:

```cpp
std::size_t longestStableWindow(
    const std::vector<int>& values,
    int limit);
```

Return the length of the longest contiguous subarray for which:

```text
maximum value - minimum value <= limit
```

The input may contain duplicates and negative values.

## Examples

```text
values = {8, 2, 4, 7},          limit = 4 -> 2
values = {10, 1, 2, 4, 7, 2},  limit = 5 -> 4
values = {4, 2, 2, 2, 4, 4},   limit = 0 -> 3
values = {},                    limit = 3 -> 0
```

## Requirements

- Use C++20.
- Target time complexity: `O(n)` amortized.
- Do not sort or modify the input.
- Use `O(n)` worst-case auxiliary space or better.
- Return `0` for an empty input.
- Return `0` when `limit` is negative.
- Avoid rescanning the current window to recompute its minimum or maximum.

## Intended pattern

Use a sliding window whose right boundary advances once through the input. Maintain enough information to obtain the current window minimum and maximum in constant time.

The intended solution uses two monotonic deques of indices:

- one for candidates for the current minimum;
- one for candidates for the current maximum.

Before coding, determine:

1. The ordering invariant of each deque.
2. Why indices are needed instead of storing only values.
3. When elements must be removed from the back.
4. When expired elements must be removed from the front.
5. How the left boundary advances when the window becomes invalid.

## Before coding

Add brief comments to `solution.cpp` explaining:

1. The brute-force approach and its complexity.
2. The invariant of each monotonic deque.
3. Why every index is inserted and removed at most once per deque.
4. The resulting amortized time complexity.
5. How duplicate values are handled correctly.

## Edge cases

Consider:

- Empty input.
- One element.
- Negative `limit`.
- `limit == 0`.
- All values equal.
- Strictly increasing or decreasing input.
- Alternating very small and very large values.
- Negative values.
- Duplicate minima or maxima leaving the window.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/003-longest-stable-window/solution.cpp \
  -o /tmp/cpp-challenge-003

/tmp/cpp-challenge-003
```

The initial scaffold compiles but its assertions intentionally fail until the TODO is implemented.

Sanitizer build:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion \
  -g -fsanitize=address,undefined -fno-omit-frame-pointer \
  tasks/003-longest-stable-window/solution.cpp \
  -o /tmp/cpp-challenge-003-san

/tmp/cpp-challenge-003-san
```