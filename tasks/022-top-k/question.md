# Task 022 — Top K Elements

## Goal

Implement:

```cpp
std::vector<int> topK(const std::vector<int>& values, std::size_t k);
```

Return the `k` largest values in descending order.

Duplicates count as separate values. If `k` is greater than the input size, return all values in descending order.

## Examples

```text
values = {5, 1, 9, 3, 9, 7}, k = 3 -> {9, 9, 7}
values = {4, 2}, k = 5 -> {4, 2}
values = {}, k = 3 -> {}
```

## Requirements

- Use C++20.
- Do not modify the input.
- Handle `k == 0`.
- Avoid sorting the entire input when `k` is much smaller than `n`.
- Target time complexity: `O(n log k)`.
- Target extra-space complexity: `O(k)` excluding the result.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/022-top-k/solution.cpp -o /tmp/cpp-challenge-022
/tmp/cpp-challenge-022
```
