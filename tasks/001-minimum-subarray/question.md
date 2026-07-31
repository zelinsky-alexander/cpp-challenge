# Task 001 — Minimum Subarray Length

## Goal

Implement:

```cpp
std::size_t minSubarrayLength(
    const std::vector<int>& values,
    int target);
```

The input contains **positive integers only**.

Return the minimum length of a contiguous subarray whose sum is greater than or equal to `target`.

Return `0` when no qualifying subarray exists.

## Examples

```text
values = {2, 3, 1, 2, 4, 3}, target = 7  -> 2
values = {1, 1, 1, 1},       target = 5  -> 0
values = {9, 1, 1},          target = 7  -> 1
values = {},                 target = 3  -> 0
```

## Requirements

- Target time complexity: `O(n)`.
- Target extra-space complexity: `O(1)`.
- Use C++20.
- Do not use a nested loop that enumerates every subarray.
- Handle possible overflow of the running sum reasonably.

## Before coding

Write brief comments in `solution.cpp` explaining:

1. The brute-force approach and its complexity.
2. The algorithmic pattern you selected.
3. The invariant maintained by the algorithm.
4. Why the positive-integer constraint matters.
5. Time and extra-space complexity.

## Tests to consider

- Empty input.
- One element below, equal to, and above the target.
- No valid subarray.
- The entire vector is the shortest valid subarray.
- The answer occurs at the beginning, middle, or end.
- A single element is the answer.
- Large values that could overflow an `int` running sum.
