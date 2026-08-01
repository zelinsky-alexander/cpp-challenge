# Task 002 — K Smallest Elements

## Goal

Implement:

```cpp
std::vector<int> kSmallest(
    const std::vector<int>& values,
    std::size_t k);
```

Return the `k` smallest elements from `values`, sorted in ascending order.

Duplicates are significant and must be preserved. For example, if the input contains three copies of `2`, more than one copy may appear in the result.

## Examples

```text
values = {7, 2, 9, 1, 5}, k = 3  -> {1, 2, 5}
values = {4, 1, 1, 3},    k = 3  -> {1, 1, 3}
values = {5, 2},           k = 0  -> {}
values = {5, 2},           k = 5  -> {2, 5}
values = {},               k = 2  -> {}
```

## Requirements

- Use C++20.
- Do not sort the entire input vector.
- Target time complexity: `O(n log k)` when `0 < k < n`.
- Target auxiliary space complexity: `O(k)`, excluding the returned vector.
- Do not modify `values`.
- Preserve duplicate values.
- Treat `k > values.size()` as requesting all available elements.

## Intended pattern

Maintain a bounded set of the best `k` candidates seen so far.

Before coding, determine:

1. Whether the bounded structure should expose its smallest or largest element at the top.
2. What condition causes an incoming value to replace the current boundary element.
3. Why the structure never needs to contain more than `k` elements.
4. Why a final ordering step is still required.

## Before coding

Add brief comments to `solution.cpp` explaining:

1. The straightforward full-sort solution and its complexity.
2. The selected STL data structure and comparator behavior.
3. The invariant maintained after processing each input element.
4. Time and auxiliary-space complexity, including final result ordering.
5. Why duplicates are handled correctly.

## Edge cases

Consider:

- Empty input.
- `k == 0`.
- `k == 1`.
- `k >= values.size()`.
- All values equal.
- Negative values.
- Duplicate values around the selection boundary.
- Input already sorted ascending or descending.

## Build and run

From the repository root:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/002-k-smallest-elements/solution.cpp \
  -o /tmp/cpp-challenge-002

/tmp/cpp-challenge-002
```

Sanitizer build:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion \
  -g -fsanitize=address,undefined -fno-omit-frame-pointer \
  tasks/002-k-smallest-elements/solution.cpp \
  -o /tmp/cpp-challenge-002-san

/tmp/cpp-challenge-002-san
```
