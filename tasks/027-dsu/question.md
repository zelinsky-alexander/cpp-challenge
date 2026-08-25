# Task 027 — Disjoint Set Union

## Goal

Implement a disjoint-set union structure:

```cpp
class DisjointSet {
public:
    explicit DisjointSet(std::size_t count);

    std::size_t find(std::size_t value);
    bool unite(std::size_t a, std::size_t b);
    bool connected(std::size_t a, std::size_t b);
    std::size_t components() const noexcept;
};
```

Initially every element is in its own set.

`unite(a, b)` returns `true` when two previously separate sets were merged and `false` when they were already connected.

## Requirements

- Use C++20.
- Use path compression.
- Use union by rank or union by size.
- Throw `std::out_of_range` for an invalid element index.
- `components()` returns the current number of distinct sets.
- Target amortized complexity per operation: effectively constant, `O(alpha(n))`.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/027-dsu/solution.cpp -o /tmp/cpp-challenge-027
/tmp/cpp-challenge-027
```
