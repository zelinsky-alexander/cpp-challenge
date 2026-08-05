# Task 007 — Manual Array Sort

## Goal

Implement:

```cpp
void sortArray(std::vector<int>& values);
```

Sort `values` in ascending order.

## Requirements

- Use C++20.
- Do not use `std::sort`, `std::stable_sort`, or any other standard-library sorting algorithm.
- Modify the input vector in place.
- Preserve duplicate values.
- Handle negative values.
- Handle empty and single-element inputs.

## Examples

```text
{7, 2, 9, 1, 5} -> {1, 2, 5, 7, 9}
{4, 1, 1, 3}    -> {1, 1, 3, 4}
{}               -> {}
{5}              -> {5}
```
